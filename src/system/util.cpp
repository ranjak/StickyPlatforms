#include "util.h"

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

}
