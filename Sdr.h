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
#include <numeric>

#include "Structs.h"
#include "Defines.h"
#include "Colours.h"
#include "Utils.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

extern std::atomic<bool> restartReader;

float iq_to_db(const Complex* data, size_t count);
float iq_to_dbfs(const Complex* data, size_t count);
void fft(Complex* data, size_t n);
void processBatch(std::unique_ptr<BufferSlot> dataNode);
void processingThread();
void sdrReadingThread(const std::string& cmd);