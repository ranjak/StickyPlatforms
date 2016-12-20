#include "util.h"
#include <iomanip>
#include <sstream>

namespace game {

template<typename T>
T clamp(T min, T value, T max)
{
  if (value < min)
    return min;
  if (value > max)
    return max;

  return value;
}

template float clamp(float min, float value, float max);

std::string formatTime(uint32_t time, bool printDecimals)
{
  int seconds = (time % 60000) / 1000;
  int minutes = time / 60000;

  std::ostringstream out;
  out << std::setw(2) << std::setfill('0') << minutes <<":";
  out << std::setw(2) << std::setfill('0') << seconds;

  if (printDecimals) {
    int centiseconds = (time % 1000) / 10;
    out << ":" << std::setw(2) << std::setfill('0') << centiseconds;
  }

  return out.str();
}

}
