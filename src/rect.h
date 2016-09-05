#ifndef RECT_H
#define RECT_H

#include "SDL.h"
#include "gamevector.h"

namespace game {


/**
 * @brief The Rect class represents a rectangle in 2D space,
 * characterized by the coordinates of its top-left corner and its size.
 */
class Rect
{
public:
  Rect();
  Rect(int x, int y, int w, int h);

  bool intersects(const Rect& other) const;
  Rect getIntersection(const Rect &other) const;
  Vector<int> getCollisionNormal(const Rect &other) const;

public:
  int x, y, w, h;
};

} // namespace game

#endif // RECT_H
