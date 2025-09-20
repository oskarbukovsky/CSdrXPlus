#include "Colours.h"

std::vector<uint8_t> colorCodes = { 16, 17, 18, 19, 20, 21, 27, 33, 39, 45, 50, 119, 82, 46, 82, 118, 154, 190, 226, 227, 226, 220, 214, 202, 196 };

std::string getColour(float value, float min, float max) {
	if (value < min) value = min;
	if (value > max) value = max;
	float ratio = (value - min) / (max - min);
	int index = static_cast<int>(2 * ratio * (colorCodes.size() - 1));
	//for (int i = 0; i <= (index - 1); i++) {
	//	std::cout << " ";
	//}
	if (index % 2 == 0) {
		return fg(colorCodes[index / 2]) + "█";
	}
	else {
		return fg(colorCodes[index / 2]) + bg(colorCodes[index / 2] + 1) + "▒";
	}
}

static void printColours() {
	std::println("Počet barev: {}", 1 + (colorCodes.size() - 1) * 2);
	auto it = colorCodes.begin();
	auto end = colorCodes.end();

	for (; it != end; ++it) {
		int c = *it;
		std::cout << fg(c) << "█";
		auto next = std::next(it);
		if (next != end) {
			int c2 = *next;
			std::cout << fg(c) << bg(c2) << "▒";
		}
	}
	std::cout << reset << std::endl;
}
static void printColour(int index) {
	if (index > (colorCodes.size() - 1) * 2)
	{
		std::cout << fg(15) << "█";
		return;
	}
	for (int i = 0; i <= (index - 1); i++) {
		std::cout << " ";
	}
	if (index % 2 == 0) {
		std::cout << fg(colorCodes[index / 2]) << "█" << reset << index << "\n";
	}
	else {
		std::cout << fg(colorCodes[index / 2]) << bg(colorCodes[index / 2] + 1) << "▒" << reset << index << "\n";
	}
}


//for (int i = 0; i < 256; i++)
//{
//	if (i % 16 == 0 && i != 0)
//		std::cout << std::endl;
//	printf("\033[38;5;%dm %3d\033[m", i, i);
//}
//std::cout << std::endl;

//printColours();
//std::cout << getColour(-27.4f, -150.0f, 5.0f) << reset << std::endl;
//std::cout << getColour(-32.0f, -150.0f, 5.0f) << reset << std::endl;
/*printColour(0);
printColour(1);
printColour(24);
printColour(25);
printColour(26);
printColour(47);
printColour(48);
printColour(49);
printColour(50);
printColour(72);*/
//printf("\n\033[38;5;16m█\033[38;5;16;48;5;17m▒\033[38;5;17m█\033[38;5;17;48;5;18m▒\033[38;5;18m█\033[38;5;18;48;5;19m▒\033[38;5;19m█\033[38;5;19;48;5;20m▒\033[38;5;20m█\033[38;5;20;48;5;21m▒\033[38;5;21m█\033[38;5;21;48;5;27m▒\033[38;5;27m█\033[38;5;27;48;5;79m▒\033[38;5;79m█\033[38;5;79;48;5;154m▒\033[38;5;154m█\033[38;5;154;48;5;118m▒\033[38;5;118m█\033[38;5;118;48;5;82m▒\033[38;5;82m█\033[m\n");
