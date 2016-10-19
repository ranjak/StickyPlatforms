#ifndef UTIL_H
#define UTIL_H

#include <cmath>

/**
 * @file util.h
 * Utility functions.
 */

namespace game {

template<typename T>
T clamp(T min, T value, T max);

template<typename T>
T distance(T val1, T val2)
{
  return std::abs(val1 - val2);
}

}



#endif // UTIL_H
