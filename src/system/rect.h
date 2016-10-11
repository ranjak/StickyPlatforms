#ifndef RECT_H
#define RECT_H

#include "gamevector.h"
#include <iostream>

namespace game {


/**
 * @brief The Rect class represents a rectangle in 2D space,
 * characterized by the coordinates of its top-left corner and its size.
 */
template<typename T>
class Rect
{
public:
  Rect();
  Rect(T x, T y, T w, T h);

  bool isValid() const;

  bool intersects(const Rect& other) const;
  bool touches(const Rect &other) const;
  bool contains(const Rect &other) const;

  Rect<T> getIntersection(const Rect &other) const;
  Vector<int> getCollisionNormal(const Rect &other) const;

  Vector<T> getCenter() const;
  void setCenter(Vector<T>);

  T distanceX(const Rect &other) const;
  T distanceY(const Rect &other) const;

  friend std::ostream &operator<<(std::ostream &os, const Rect &r)
  {
    return os << "{"<<r.x<<","<<r.y<<","<<r.w<<","<<r.h<<"}";
  }

public:
  T x, y, w, h;
};

} // namespace game

#endif // RECT_H
