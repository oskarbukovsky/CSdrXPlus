#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <iomanip>

#include "Defines.h"
#include "Colours.h"
#include "Structs.h"
#include "Utils.h"

void printOutlines(std::vector<std::vector<std::string>>& output, int width, int height);
void printFrequency(std::vector<std::vector<std::string>>& output, int width, long double frequency);
void printBands(std::vector<std::vector<std::string>>& output, int width, std::vector<Band> bands, long double frequency, long double sampleRate);
void printWaterfall(std::vector<std::vector<std::string>>& output, int width);
void printWaterfallScale(std::vector<std::vector<std::string>>& output, int width, float min, float max);
void printTo(std::vector<std::vector<std::string>>& output, int row, int col, std::string text);