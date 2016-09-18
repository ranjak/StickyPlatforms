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

const Color Color::BLACK(0, 0, 0);
const Color Color::WHITE(255, 255, 255);
const Color Color::RED(255, 0, 0);
const Color Color::GREEN(0, 255, 0);
const Color Color::BLUE(0, 0, 255);

} // namespace game
