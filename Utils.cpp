#include "Utils.h"

int width = 0, height = 0;
std::vector<std::string> waterfallPrint;

std::vector<Band> readBands(const std::string& filename) {
	std::vector<Band> bands;
	std::ifstream f(filename);
	std::string line;
	while (std::getline(f, line)) {
		if (line.empty()) continue;

		Band tmpBand;
		std::stringstream stream(line);
		int low, high;

		std::getline(stream, tmpBand.name, ':');

		stream >> low;
		stream.ignore(3);
		stream >> high;

		tmpBand.low = low;
		tmpBand.high = high;

		bands.push_back(tmpBand);
	}
	return bands;
}

std::string getHumanReadableFrequency(long double frequency) {
	const char* units[] = { "Hz", "kHz", "MHz", "GHz" };
	int unitIndex = 0;
	while (frequency >= 1000.0 && unitIndex < 3) {
		frequency /= 1000;
		unitIndex++;
	}
	return std::to_string(frequency) + " " + units[unitIndex];
}

#ifdef _WIN32
void activateVirtualTerminal()
{
	HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(handleOut, &consoleMode);
	consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(handleOut, consoleMode);
}
#endif

void get_terminal_size(int& width, int& height) {
#if defined(_WIN32)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
	height = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#elif defined(__linux__)
	struct winsize w;
	ioctl(fileno(stdout), TIOCGWINSZ, &w);
	width = (int)(w.ws_col);
	height = (int)(w.ws_row);
#endif // Windows/Linux
}

std::string getCommand(long double frequency, long double buffer_size, int gain) {
	return std::format("rtl_sdr -f {} -s {} -n 0 -g {} - 2>nul", frequency - (buffer_size / 2 /* I+Q */ / 2 /* Mid */), buffer_size, gain);;
}

std::string printOutputToString(const std::vector<std::vector<std::string>>& printOutput) {
	std::ostringstream oss;
	for (const auto& row : printOutput) {
		for (const auto& cell : row) {
			oss << cell;
		}
		oss << '\n';
	}
	return oss.str();
}