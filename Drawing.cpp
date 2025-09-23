#include "Drawing.h"

//Pozice                 0    1    2    3    4    5    6    7    8	  9    10   11   12   13   14   15   16
std::string box[17] = { "╔", "╗", "╝", "╚", "═", "║", "╟", "╢", "╤", "╧", "─", "│", "┬", "┴", "├", "┤", "┼" };

void printOutlines(std::vector<std::vector<std::string>>& output, int width, int height) {
	std::ostringstream oss;
	oss << "\033[10;" << width - CONTROL_SIZE << "H";
	oss << "width=" << width << ", height=" << height;
	oss << "\033[0;0H" << box[0];
	for (int i = 1; i < width - 1; i++) {
		if (i == width - CONTROL_SIZE - 2) {
			oss << box[8];
		}
		else {
			oss << box[4];
		}
	}
	printTo(output, 1, 1, box[0]);
	for (int i = 2; i < width - 1; i++) {
		if (i == width - CONTROL_SIZE - 1) {
			printTo(output, 1, i, box[8]);
		}
		else {
			printTo(output, 1, i, box[4]);
		}
	}
	printTo(output, 1, width - 2, box[1]);
	printTo(output, 1, width - 1, box[1]);
	printTo(output, 1, width, box[1]);
	for (int i = 2; i < height; i++) {
		if (i == 4) {
			printTo(output, i, 0, box[6]);
			printTo(output, i, width - 1, box[7]);
			for (int j = 1; j < width - CONTROL_SIZE - 2; j++) {
				printTo(output, i, j, box[10]);
			}
			printTo(output, i, width - CONTROL_SIZE - 2, box[16]);
			for (int j = 0; j < CONTROL_SIZE; j++) {
				printTo(output, i, width - CONTROL_SIZE + j - 1, box[10]);
			}
		}
		else if (i == 7 || i == 9) {
			printTo(output, i, 0, box[5]);
			printTo(output, i, width - 1, box[7]);
			printTo(output, i, width - CONTROL_SIZE - 2, box[14]);
			for (int j = 0; j < CONTROL_SIZE; j++) {
				printTo(output, i, width - CONTROL_SIZE + j - 1, box[10]);
			}
		}
		else {
			printTo(output, i, 0, box[5]);
			printTo(output, i, width - CONTROL_SIZE - 2, box[11]);
			printTo(output, i, width - 1, box[5]);
		}
	}
	printTo(output, height - 1, 0, box[3]);
	for (int i = 1; i < width - 1; i++) {
		if (i == width - CONTROL_SIZE - 2) {
			printTo(output, height - 1, i, box[9]);
		}
		else {
			printTo(output, height - 1, i, box[4]);
		}
	}
	printTo(output, height - 1, width - 1, box[2]);

	oss << box[1];
	for (int i = 2; i < height; i++) {
		if (i == 4) {
			oss << esc << i << ";0H" << box[6] << esc << i << ";" << width << "H" << box[7];
			oss << esc << i << ";2H";
			for (int j = 2; j < width - CONTROL_SIZE - 1; j++) {
				oss << box[10];
			}
			oss << box[16];
			oss << esc << i << ";" << width - CONTROL_SIZE << "H";
			for (int j = 0; j < CONTROL_SIZE; j++) {
				oss << box[10];
			}
		}
		else if (i == 7 || i == 9) {
			oss << esc << i << ";0H" << box[6] << esc << i << ";" << width << "H" << box[7];
			oss << esc << i << ";" << width - CONTROL_SIZE - 1 << "H";
			oss << box[14];
			oss << esc << i << ";" << width - CONTROL_SIZE << "H";
			for (int j = 0; j < CONTROL_SIZE; j++) {
				oss << box[10];
			}
		}
		else {
			oss << esc << i << ";0H" << box[5] << esc << i << ";" << width - CONTROL_SIZE - 1 << "H" << box[11] << esc << i << ";" << width << "H" << box[5];
		}
	}
	oss << esc << height << ";0H" << box[3];
	for (int i = 1; i < width - 1; i++) {
		if (i == width - CONTROL_SIZE - 2) {
			oss << box[9];
		}
		else {
			oss << box[4];
		}
	}
	oss << box[2];
	//std::cout << oss.str();
	//std::cout.flush();
}

void printWaterfallScale(std::vector<std::vector<std::string>>& output, int width, float min, float max) {
	//std::cout << "\033[5;" << width - CONTROL_SIZE / 2 - 8 << "H" << "Waterfall scale:";
	//std::cout << "\033[6;" << width - CONTROL_SIZE + 1 << "HMinimum: " << std::setw(4) << std::setfill(' ') << min << " dB";
	//std::cout << "\033[6;" << width - 18 << "HMaximum: " << std::setw(4) << std::setfill(' ') << max << " dB ";

	printTo(output, 5, width - CONTROL_SIZE / 2 - 8, "Waterfall scale:");
	//std::cout.flush();
}
void printFrequency(std::vector<std::vector<std::string>>& output, int width, long double frequency) {
	std::cout << "\033[2;" << width - CONTROL_SIZE / 2 - 5 << "H" << "Frequency:" << "\033[3;" << width - CONTROL_SIZE / 2 - 6 << "H" << getHumanReadableFrequency(frequency);
	//std::cout << "\033[5;" << width - CONTROL_SIZE << "HWF min: " << waterfall_min << " dB";
	//std::cout << "\033[6;" << width - CONTROL_SIZE << "HWF max: " << waterfall_max << " dB";
	//std::cout << "\033[7;" << width - CONTROL_SIZE << "HD.Gain: " << gain << " dB";
	//std::cout.flush();
	std::cout << "\033[30;" << width - CONTROL_SIZE << "HGain: " << std::setw(2) << std::setfill(' ') << gain;
}

void printBands(std::vector<std::vector<std::string>>& output, int width, std::vector<Band> bands, long double frequency, long double sampleRate) {
	std::cout << "\033[2;2H";
	for (int i = 1; i < width - CONTROL_SIZE - 2; i++) {
		std::cout << "#";
		//TODO
	}
	std::cout << "\033[3;" << (width - CONTROL_SIZE) / 2 << "H" << fg(220) << "▗█▖" << reset;
	//std::cout.flush();
}

extern std::mutex waterfall_mutex; // Přidej extern deklaraci

void printWaterfall(std::vector<std::vector<std::string>>& output, int width) {
	int startLine = 5;
	size_t maxLines = static_cast<size_t>(height) - startLine;
	std::ostringstream oss;
	{
		std::lock_guard<std::mutex> lock(waterfall_mutex);
		size_t linesToPrint = std::min<size_t>(maxLines, waterfallPrint.size());
		for (int i = 0; i < linesToPrint; i++) {
			int idx = (int)waterfallPrint.size() - 1 - i;
			oss << esc << (startLine + i) << ";2H" << waterfallPrint[idx] << reset;
		}
	}
	std::cout << oss.str();
	//std::cout.flush();
}

void printTo(std::vector<std::vector<std::string>>& output, int row, int col, std::string text) {
	int outPos = 0;
	for (size_t i = 0; i < text.size();) {
		// Detekce escape sekvence
		if (text[i] == '\033' && i + 1 < text.size() && text[i + 1] == '[') {
			i += 2;
			while (i < text.size() && !isalpha(static_cast<unsigned char>(text[i]))) i++;
			if (i < text.size()) i++; // přeskoč ukončující znak
			continue;
		}
		// Pokud je znak písmeno nebo číslice, zapiš ho samostatně
		if (isalnum(static_cast<unsigned char>(text[i]))) {
			if (row >= 0 && row < static_cast<int>(output.size()) &&
				col + outPos >= 0 && col + outPos < static_cast<int>(output[row].size())) {
				output[row][col + outPos] = text.substr(i, 1);
			}
			i++;
			outPos++;
		}
		else {
			// Najdi délku bloku nepovolených znaků
			size_t start = i;
			while (i < text.size() && !isalnum(static_cast<unsigned char>(text[i]))) {
				// přeskoč escape sekvenci uvnitř bloku
				if (text[i] == '\033' && i + 1 < text.size() && text[i + 1] == '[') {
					i += 2;
					while (i < text.size() && !isalpha(static_cast<unsigned char>(text[i]))) i++;
					if (i < text.size()) i++;
				}
				else {
					i++;
				}
			}
			if (row >= 0 && row < static_cast<int>(output.size()) &&
				col + outPos >= 0 && col + outPos < static_cast<int>(output[row].size())) {
				output[row][col + outPos] = text.substr(start, i - start);
			}
			outPos++;
		}
	}
}