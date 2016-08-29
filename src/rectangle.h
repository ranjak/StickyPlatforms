#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "graphics.h"
#include "color.h"
#include <cstdint>

namespace game {


class Rectangle : public Graphics
{
public:
  Rectangle();

  Rectangle(int x, int y, int w, int h);

  void draw(Display& target) const override;

  void setPos(int x, int y);
  void setSize(int w, int h);
  void setColor(const Color& color);

private:
  // Fill color of the cube
  Color color;
  // Size and position in window coordinates
  SDL_Rect sizePos;
};

} // namespace game

#endif // RECTANGLE_H
