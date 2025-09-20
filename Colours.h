#pragma once

#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <print>

std::string getColour(float value, float min, float max);

constexpr std::string_view esc = "\033[";

inline std::string fg(int c) {
    std::ostringstream oss;
    oss << esc << "38;5;" << c << "m";
    return oss.str();
}
inline std::string bg(int c) {
    std::ostringstream oss;
    oss << esc << "48;5;" << c << "m";
    return oss.str();
}
inline std::string reset = "\033[m"; 