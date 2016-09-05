#include "rect.h"
#include <cmath>
#include <algorithm>

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

Rect Rect::getIntersection(const Rect &other) const
{
  if (!intersects(other))
    return Rect(0, 0, 0, 0);

  // Intersection coordinates
  int ix = std::max(x, other.x);
  int iy = std::max(y, other.y);
  int iw = std::min(x+w, other.x+other.w) - ix;
  int ih = std::min(y+h, other.y+other.h) - iy;

  return Rect(ix, iy, iw, ih);
}

Vector<int> Rect::getCollisionNormal(const Rect &other) const
{
  Rect intersection = getIntersection(other);

  // Normal coordinates
  // Check the region of the collision by comparing the two rectangles' centers
  int nx = ((x+w) / 2 < (other.x+other.w) / 2) ? -1 : 1;
  int ny = ((y+h) / 2 < (other.y+other.h) / 2) ? -1 : 1;

  // If the intersection is wider than tall, the collision has to be on the top or bottom
  if (intersection.w >= intersection.h)
    return Vector<int>(0, ny);
  else
    return Vector<int>(nx, 0);
}


} // namespace game
