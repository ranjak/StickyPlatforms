#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <string>

namespace game {

class Color
{
public:
  static const Color BLACK, WHITE, RED, GREEN, BLUE;

  Color();
  Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a=255);

  Color(const std::string &color);

public:
  std::uint8_t r, g, b, a;
};

} // namepsace game

#endif // COLOR_H
