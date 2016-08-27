#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "display.h"
#include "SDL.h"

namespace game {


class Graphics
{
public:
  Graphics();

  virtual void draw(Display& target) const = 0;

};

} //namespace game

#endif // GRAPHICS_H
