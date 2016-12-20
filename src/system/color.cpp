#include "color.h"

namespace game {

using std::uint8_t;

Color::Color() :
  Color(255, 255, 255)
{

}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
  r(r), g(g), b(b), a(a)
{

}

Color::Color(const std::string& color)
{
  // Skip leading # if present
  size_t dataStart = color.find('#');
  dataStart = (dataStart == std::string::npos) ? 0 : dataStart + 1;

  // Extract individual components
  std::string component = color.substr(dataStart, 2);
  r = static_cast<uint8_t>(std::stoi(component, nullptr, 16));
  component = color.substr(dataStart + 2, 2);
  g = static_cast<uint8_t>(std::stoi(component, nullptr, 16));
  component = color.substr(dataStart + 4, 2);
  b = static_cast<uint8_t>(std::stoi(component, nullptr, 16));

  // Alpha defaults to 255 if not given
  if (dataStart + 6 < color.length()) {
    component = color.substr(dataStart + 6);
    a = static_cast<uint8_t>(std::stoi(component, nullptr, 16));
  }
  else {
    a = 255;
  }
}

const Color Color::BLACK(0, 0, 0);
const Color Color::WHITE(255, 255, 255);
const Color Color::RED(255, 0, 0);
const Color Color::GREEN(0, 255, 0);
const Color Color::BLUE(0, 0, 255);

} // namespace game
