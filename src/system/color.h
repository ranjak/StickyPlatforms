#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

namespace game {

class Color
{
public:
  static const Color BLACK, WHITE, RED, GREEN, BLUE;

  Color();
  Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a=255);

public:
  std::uint8_t r, g, b, a;
};

} // namepsace game

#endif // COLOR_H
