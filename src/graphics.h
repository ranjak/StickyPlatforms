#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "display.h"

namespace game {


class Graphics
{
public:
  Graphics();

  virtual void draw(Display& target, int x, int y) const = 0;

};

} //namespace game

#endif // GRAPHICS_H
