#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "display.h"

namespace game {


class Graphics
{
public:
  Graphics();

  void draw(const Display& target);
};

} //namespace game

#endif // GRAPHICS_H
