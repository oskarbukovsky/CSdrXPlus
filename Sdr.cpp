#include "Sdr.h"

std::queue<std::unique_ptr<BufferSlot>> dataQueue;
std::mutex queue_mutex;
std::condition_variable queue_cv;
bool finished = false;

// Přidej mutex pro waterfallPrint
std::mutex waterfall_mutex;

float iq_to_db(const Complex* data, size_t count) {
	float sum = 0.0;
	for (size_t i = 0; i < count; ++i) {
		int i_val = static_cast<int>(data[i].I) - 127;
		int q_val = static_cast<int>(data[i].Q) - 127;
		sum += i_val * i_val + q_val * q_val;
	}
	float avg_power = sum / count;
	float max_power = 2.0 * 127.0 * 127.0; // max hodnota pro I/Q = 127
	float div = avg_power / max_power;
	float mult = 10.0f * std::log10f(div);
	return mult;
}

float iq_to_dbfs(const Complex* data, size_t count) {
	float sum = 0.0f;
	for (size_t i = 0; i < count; ++i) {
		float i_val = (static_cast<int>(data[i].I) - 128) / 128.0f; // [-1,1]
		float q_val = (static_cast<int>(data[i].Q) - 128) / 128.0f; // [-1,1]
		sum += i_val * i_val + q_val * q_val;
	}
	float rms = std::sqrt(sum / count);
	float max_rms = std::sqrt(2.0f); // max RMS pro I/Q = ±1
	return 20.0f * std::log10(rms / max_rms);
}

static void fft(Complex* data, size_t n) {
    if (n <= 1) return;

    // Bit reversal
    size_t j = 0;
    for (size_t i = 1; i < n; ++i) {
        size_t bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) std::swap(data[i], data[j]);
    }

    // Iterative Cooley-Tukey
    for (size_t len = 2; len <= n; len <<= 1) {
        float ang = -2.0f * float(M_PI) / float(len);
        float wlen_re = std::cos(ang);
        float wlen_im = std::sin(ang);
        for (size_t i = 0; i < n; i += len) {
            float w_re = 1.0f, w_im = 0.0f;
            for (size_t j = 0; j < len / 2; ++j) {
                // u = data[i + j]
                float u_re = data[i + j].I;
                float u_im = data[i + j].Q;
                // v = w * data[i + j + len/2]
                float v_re = w_re * data[i + j + len / 2].I - w_im * data[i + j + len / 2].Q;
                float v_im = w_re * data[i + j + len / 2].Q + w_im * data[i + j + len / 2].I;

                // data[i + j] = u + v
                data[i + j].I = u_re + v_re;
                data[i + j].Q = u_im + v_im;
                // data[i + j + len/2] = u - v
                data[i + j + len / 2].I = u_re - v_re;
                data[i + j + len / 2].Q = u_im - v_im;

                // w *= wlen
                float next_w_re = w_re * wlen_re - w_im * wlen_im;
                float next_w_im = w_re * wlen_im + w_im * wlen_re;
                w_re = next_w_re;
                w_im = next_w_im;
            }
        }
    }
}

std::chrono::steady_clock::time_point lastPrint = std::chrono::steady_clock::now();
void processBatch(std::unique_ptr<BufferSlot> dataNode) {
	size_t N = dataNode->count;
	if (N == 0) return;

	size_t n_fft = 1;
	while (n_fft < N) n_fft <<= 1;
	if (n_fft > N) n_fft >>= 1;

	// FFT přímo nad Complex
	fft(dataNode->data.get(), n_fft);

	// Výpočet spektra (amplitudy v dBFS)
	std::vector<float> spectrum(n_fft / 2);
	float max_amplitude = std::sqrt(127.0f * 127.0f + 127.0f * 127.0f);
	for (size_t i = 0; i < n_fft / 2; ++i) {
		float mag = std::hypot(dataNode->data[i].I, dataNode->data[i].Q);
		float dbfs = 20.0f * std::log10(mag / max_amplitude + 1e-12f);
		spectrum[i] = dbfs;
	}
	float max_dbfs = *std::max_element(spectrum.begin(), spectrum.end());
	float min_dbfs = *std::min_element(spectrum.begin(), spectrum.end());
	float avg_dbfs = std::accumulate(spectrum.begin(), spectrum.end(), 0.0f) / spectrum.size();

	// Normalizace spektra (volitelné)
	for (auto& v : spectrum) v -= max_dbfs;

	//// jednoduchý fftshift
	std::rotate(spectrum.begin(), spectrum.begin() + sizeof(spectrum.begin()) / 2, spectrum.end());

	// Vykreslení spektra (barevně)
	std::string toPrint;
	size_t step = std::max<size_t>(1, spectrum.size() / (static_cast<size_t>(width) - CONTROL_SIZE - 4));
	for (size_t i = 0; i < spectrum.size(); i += step) {
		toPrint.append(getColour(spectrum[i], waterfall_min, waterfall_max));
	}
	{
		std::lock_guard<std::mutex> lock(waterfall_mutex);
		waterfallPrint.push_back(toPrint);
		if (waterfallPrint.size() > static_cast<size_t>(height) - 5) {
			waterfallPrint.erase(waterfallPrint.begin());
		}
	}
	auto now = std::chrono::steady_clock::now();
	std::cout << "\033[21;" << width - CONTROL_SIZE << "HCalc: " << std::setw(4) << std::setfill(' ') << std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPrint).count() << "ms" << reset;
	std::cout << "\033[27;" << width - CONTROL_SIZE << "HMaxDb: " << max_dbfs;
	std::cout << "\033[28;" << width - CONTROL_SIZE << "HMinDb: " << min_dbfs;
	std::cout << "\033[29;" << width - CONTROL_SIZE << "HAvgDb: " << avg_dbfs;
	lastPrint = now;
}

void processingThread() {
	while (true) {
		std::unique_ptr<BufferSlot> dataNode;
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			queue_cv.wait(lock, [] { return !dataQueue.empty() || finished; });
			if (dataQueue.empty() && finished) break;
			dataNode = std::move(dataQueue.front());
			dataQueue.pop();
		}
		processBatch(std::move(dataNode));
	}
}

std::atomic<bool> restartReader = false;
std::chrono::steady_clock::time_point lastRead = std::chrono::steady_clock::now();
void sdrReadingThread(const std::string& cmd) {
	FILE* pipe = popen(cmd.c_str(), "rb");
	if (!pipe) {
		std::cerr << "Nepodařilo se spustit rtl_sdr\n";
		finished = true;
		queue_cv.notify_all();
		return;
	}
	while (true) {
		if (restartReader) {
			pclose(pipe);
			restartReader = false;
			pipe = popen(cmd.c_str(), "rb");
			if (!pipe) {
				std::cerr << "Nepodařilo se spustit rtl_sdr\n";
				finished = true;
				queue_cv.notify_all();
				return;
			}
		}
		auto dataNode = std::make_unique<BufferSlot>(READ_BUFFER_SIZE / sizeof(Complex));
		std::vector<unsigned char> rawBuf(READ_BUFFER_SIZE);
		size_t bytesRead = fread(rawBuf.data(), 1, READ_BUFFER_SIZE, pipe);
		dataNode->count = bytesRead / sizeof(Complex);

		// Převod do Complex s odečtením 127
		for (size_t i = 0; i < dataNode->count; ++i) {
			dataNode->data[i].I = static_cast<float>(rawBuf[2 * i]) - 127.0f;
			dataNode->data[i].Q = static_cast<float>(rawBuf[2 * i + 1]) - 127.0f;
		}

		if (bytesRead == 0) {
			if (feof(pipe)) {
				std::println("Konec dat ze SDR.");
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				continue;
			}
			else {
				std::cerr << "Chyba při čtení ze SDR.\n";
			}
			break;
		}
		{
			std::lock_guard<std::mutex> lock(queue_mutex);
			dataQueue.push(std::move(dataNode));
		}
		queue_cv.notify_one();
		auto now = std::chrono::steady_clock::now();
		std::cout << "\033[22;" << width - CONTROL_SIZE << "HRead: " << std::setw(4) << std::setfill(' ') << std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRead).count() << "ms" << reset;
		lastRead = now;
	}
	pclose(pipe);
	finished = true;
	queue_cv.notify_all();
}