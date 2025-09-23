#include "CSdrXPlus.h"

int main()
{
	std::vector<Band> bands = readBands("bands.txt");
	std::string cmd = getCommand(frequency, READ_BUFFER_SIZE, gain);
	std::vector<std::thread> workers;
	for (int i = 0; i < PROCESSING_WORKERS; ++i) {
		workers.emplace_back(processingThread);
	}

	std::thread reader(sdrReadingThread, std::cref(cmd));
	reader.detach();

	for (auto& t : workers) t.detach();

	std::vector<std::vector<std::string>> printOutput;

	for (int i = 0; i < height; i++) {
		printOutput.emplace_back();
		for (int j = 0; j < width; j++) {
			printOutput[i].emplace_back(" ");
		}
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0) {
				printOutput[i][j] = "\033[" + std::to_string(i) + ";0H";
			}
			else {
			printOutput[i][j] = " ";
			}
		}
	}

	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	printOutlines(printOutput, width, height);
	//printBands(printOutput, width, bands, frequency, READ_BUFFER_SIZE / 2);
	int beforeAGC = gain;
	bool AGC = false;
	while (!stopFlag) {
		//printFrequency(printOutput, width, frequency);
		//printWaterfallScale(printOutput, width, waterfall_min, waterfall_max);
		//printWaterfall(printOutput, width);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		auto now = std::chrono::steady_clock::now();
		std::cout << "\033[20;" << width - CONTROL_SIZE << "HReprint: " << std::setw(4) << std::setfill(' ') << std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count() << "ms";
		//std::cout.flush();
		lastTime = now;
		if (_kbhit()) {
			int c = _getch();
			if (c == 0 || c == 224) {  // speciální klávesa
				int code = _getch();
				switch (code) {
				case 72:
					std::cout << "Šipka nahoru\n";
					break;
				case 80:
					std::cout << "Šipka dolů\n";
					break;
				case 75:
					std::cout << "Šipka vlevo\n";
					break;
				case 77:
					std::cout << "Šipka vpravo\n";
					break;
				default:
					break;
				}
			}
			else {
				switch (c) {
				case 'q':
					stopFlag = 1;
					break;
				case '8':
					if (gain < 48) {
						gain++;
						cmd = getCommand(frequency, READ_BUFFER_SIZE, gain);
						restartReader = true;
					}
					break;
				case '5':
					if (gain > 1)
					{
						gain--;
						cmd = getCommand(frequency, READ_BUFFER_SIZE, gain);
						restartReader = true;
					}
					break;
				case '2':
					if (AGC) {
						AGC = false;
						gain = beforeAGC;
						cmd = getCommand(frequency, READ_BUFFER_SIZE, gain);
						restartReader = true;
					}
					else {
						AGC = true;
						beforeAGC = gain;
						gain = 0;
						cmd = getCommand(frequency, READ_BUFFER_SIZE, gain);
						restartReader = true;
					}
					break;
				case '7':
					if (waterfall_min + 1.0f < waterfall_max) {
						waterfall_min += 1.0f;
					}
					break;
				case '4':
					waterfall_min -= 1.0f;
					break;
				case '9':
					waterfall_max += 1.0f;
					break;
				case '6':
					if (waterfall_max - 1.0f > waterfall_min) {
						waterfall_max -= 1.0f;
					}
					break;
				case '1':
					frequency -= 100000; // 100 kHz
					if (frequency < 1000000) frequency = 1000000;
					cmd = getCommand(frequency, READ_BUFFER_SIZE, gain);
					restartReader = true;
					printBands(printOutput, width, bands, frequency, READ_BUFFER_SIZE / 2);
					break;
				case '3':
					frequency += 100000; // 100 kHz
					if (frequency > 1300000000) {
						frequency = 1300000000;
						break;
					}
					cmd = getCommand(frequency, READ_BUFFER_SIZE, gain);
					restartReader = true;
					printBands(printOutput, width, bands, frequency, READ_BUFFER_SIZE / 2);
					break;
				}
			}
		}
		std::cout << printOutputToString(printOutput);
		std::cout.flush();
	}
	std::cout << esc << height + 1 << ";0H" << std::endl;
	std::cout.flush();
	return 0;
}