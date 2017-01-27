#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <string>

/**
 * @file util.h
 * Utility functions.
 */

namespace game {

constexpr double PId = 3.141592653589793;
constexpr float PIf = 3.1415927f;

constexpr float toRad(float deg) { return deg * (2.f*PIf / 360.f); }

template<typename T>
T clamp(T min, T value, T max);

template<typename T>
T distance(T val1, T val2)
{
  return std::abs(val1 - val2);
}

std::string formatTime(std::uint32_t time, bool printDecimals=true);

int getRandom(int min, int max);

}



#endif // UTIL_H
