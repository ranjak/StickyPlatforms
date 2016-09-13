#include "rect.h"
#include <cmath>
#include <algorithm>

namespace game {


template<typename T>
Rect<T>::Rect() :
  x(), y(), w(), h()
{

}

template<typename T>
Rect<T>::Rect(T x, T y, T w, T h) :
  x(x), y(y), w(w), h(h)
{

}

template<typename T>
bool Rect<T>::intersects(const Rect &other) const
{
  return !(x + w <= other.x || x >= other.x + other.w)
      && !(y + h <= other.y || y >= other.y + other.h);
}

template<typename T>
bool Rect<T>::touches(const Rect &other) const
{
  return !(x + w < other.x || x > other.x + other.w)
      && !(y + h < other.y || y > other.y + other.h);
}

template<typename T>
Rect<T> Rect<T>::getIntersection(const Rect &other) const
{
  if (!intersects(other))
    return Rect();

  // Intersection coordinates
  T ix = std::max(x, other.x);
  T iy = std::max(y, other.y);
  T iw = std::min(x+w, other.x+other.w) - ix;
  T ih = std::min(y+h, other.y+other.h) - iy;

  return Rect(ix, iy, iw, ih);
}

template<typename T>
Vector<int> Rect<T>::getCollisionNormal(const Rect &other) const
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

template<typename T>
Vector<T> Rect<T>::getCenter() const
{
  return Vector<T>(x + (w/2), y + (h/2));
}

template<typename T>
void Rect<T>::setCenter(Vector<T> center)
{
  x = center.x - (w / 2);
  y = center.y - (h / 2);
}


// Template definitions for needed Rect specializations
template class Rect<int>;
template class Rect<float>;

} // namespace game
