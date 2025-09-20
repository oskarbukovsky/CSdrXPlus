#pragma once

//constexpr long double FREQUENCY = 91900 * 1000; // kHz * 1000
constexpr long double FREQUENCY = 88200 * 1000; // kHz * 1000
constexpr int         BANDTWIDTH = 1000; // ksps
constexpr int         GAIN = 32;
constexpr float       WATERFALL_MIN = -35.0f;
constexpr float       WATERFALL_MAX = 0.0f;

constexpr int         CONTROL_SIZE = 45;
constexpr int         PROCESSING_WORKERS = 1;
constexpr long double M_PI = 3.14159265358979323846l;
constexpr size_t      READ_BUFFER_SIZE = BANDTWIDTH * 1000 * 2;

extern long double  frequency;
extern int          gain;
extern float 	    waterfall_min;
extern float 	    waterfall_max;