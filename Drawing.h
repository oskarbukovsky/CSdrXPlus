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

void printOutlines(int width, int height);
void printFrequency(int width, long double frequency);
void printBands(int width, std::vector<Band> bands, long double frequency, long double sampleRate);
void printWaterfall(int width);
void printWaterfallScale(int width, float min, float max);