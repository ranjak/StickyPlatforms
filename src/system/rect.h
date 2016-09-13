#ifndef RECT_H
#define RECT_H

#include "gamevector.h"

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

  bool intersects(const Rect& other) const;
  bool touches(const Rect &other) const;

  Rect<T> getIntersection(const Rect &other) const;
  Vector<int> getCollisionNormal(const Rect &other) const;

  Vector<T> getCenter() const;
  void setCenter(Vector<T>);

public:
  T x, y, w, h;
};

} // namespace game

#endif // RECT_H
