#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <print>
#include <array>
#include <string>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <complex>
#include <fstream> 
#include <csignal>
#include <cstdlib>

#include "tchar.h"
#include <conio.h>

#include "Colours.h"
#include "Defines.h"
#include "Structs.h"
#include "Utils.h"
#include "Drawing.h"
#include "Sdr.h"

class ConsoleInitializer {
public:
	ConsoleInitializer();
};

// Globální instance, která zajistí spuštění kódu před main()
extern ConsoleInitializer g_consoleInitializer;

ConsoleInitializer g_consoleInitializer; // Definice globální instance

volatile std::sig_atomic_t stopFlag = 0;

void exitHandler(int signum) {
	stopFlag = 1;
}

ConsoleInitializer::ConsoleInitializer()
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	HWND hwnd = GetConsoleWindow();
	HWND owner = GetWindow(hwnd, GW_OWNER);
	if (owner == NULL) {
		// Windows 10
		ShowWindow(hwnd, SW_MAXIMIZE);
	}
	else {
		// Windows 11
		ShowWindow(owner, SW_MAXIMIZE);
	}
#endif
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	get_terminal_size(width, height);
	for (int i = 0; i < height + 1; i++) {
		std::cout << "\n";
	}
	std::signal(SIGINT, exitHandler);
	std::cout.setf(std::ios::fmtflags(0), std::ios::unitbuf);
}