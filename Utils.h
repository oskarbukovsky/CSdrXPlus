#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#elif defined(__linux__)
#include <sys/ioctl.h>
constexpr int ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004;
#endif // Windows/Linux

#include "Structs.h"

extern int width, height;
extern std::vector<std::string> waterfallPrint;

std::vector<Band> readBands(const std::string& filename);
std::string getHumanReadableFrequency(long double frequency);

#ifdef _WIN32
void activateVirtualTerminal();
#endif

void get_terminal_size(int& width, int& height);
std::string getCommand(long double frequency, long double buffer_size, int gain);
std::string printOutputToString(const std::vector<std::vector<std::string>>& printOutput);