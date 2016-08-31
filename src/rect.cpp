#include "rect.h"

namespace game {


Rect::Rect() :
  Rect(0, 0, 0, 0)
{

}

Rect::Rect(int x, int y, int w, int h) :
  x(x), y(y), w(w), h(h)
{

}

bool Rect::intersects(const Rect &other) const
{
  return !(x + w <= other.x || x >= other.x + other.w)
      && !(y + h <= other.y || y >= other.y + other.h);
}


} // namespace game
