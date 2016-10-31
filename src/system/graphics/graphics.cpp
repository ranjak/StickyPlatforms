#include "graphics.h"
#include "gamevector.h"
#include "rect.h"

namespace game {


Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

void Graphics::draw(Display &target, const Vector<int> &pos)
{
  draw(target, pos.x, pos.y);
}

void Graphics::draw(Display &target, float x, float y)
{
  draw(target, static_cast<int>(x), static_cast<int>(y));
}

void Graphics::draw(Display &target, const Vector<float> &pos)
{
  draw(target, static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void Graphics::draw(Display &target, const Rect<float> &dest)
{
  draw(target, Rect<int>(static_cast<int>(dest.x), static_cast<int>(dest.y), static_cast<int>(dest.w), static_cast<int>(dest.h)));
}

} // namespace game
