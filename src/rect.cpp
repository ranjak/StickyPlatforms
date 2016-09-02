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
  // Check intersection, first on x and then on y axis.
  // Intersection must be true for both. Return early if a signle one isn't.
  if (x + w < other.x || x > other.x + other.w)
    return false;

  return !(y + h < other.y || y > other.y + other.h);
}


} // namespace game
