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
bool Rect<T>::isValid() const
{
  return w > 0 && h > 0;
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
bool Rect<T>::contains(const Rect &other) const
{
  return (x <= other.x && x+w >= other.x+other.w)
      && (y <= other.y && y+h >= other.y+other.h);
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
  Vector<int> normal(0, 0);
  Vector<T> center = getCenter();
  Vector<T> otherCenter = other.getCenter();

  if (std::abs(center.y - otherCenter.y) < h/2 + other.h/2) {
    normal.x = (x < other.x) ? -1 : 1;
  }
  else if (std::abs(center.x - otherCenter.x) < w/2 + other.w/2) {
    normal.y = (y < other.y) ? -1 : 1;
  }

  return normal;
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

template<typename T>
T Rect<T>::distanceX(const Rect &other) const
{
  if (x + w < other.x)
    return other.x - (x + w);

  else if (other.x + other.w < x)
    return x - (other.x + other.w);

  // Overlap or touch
  else
    return 0;
}

template<typename T>
T Rect<T>::distanceY(const Rect &other) const
{
  if (y + h < other.y)
    return other.y - (y + h);

  else if (other.y + other.h < y)
    return y - (other.y + other.h);

  // Overlap or touch
  else
    return 0;
}

// Template definitions for needed Rect specializations
template class Rect<int>;
template class Rect<float>;

} // namespace game
