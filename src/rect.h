#ifndef RECT_H
#define RECT_H

#include "SDL.h"

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

  int x, y, w, h;
};

} // namespace game

#endif // RECT_H
