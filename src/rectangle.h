#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "graphics.h"
#include "color.h"
#include <cstdint>

namespace game {


class Rectangle : public Graphics
{
public:
  Rectangle(int w=0, int h=0);

  void draw(Display& target, int x, int y) const override;

  void setSize(int w, int h);
  void setColor(const Color& mColor);

private:
  // Fill color of the rectangle
  Color mColor;
  // Size in window coordinates
  int w;
  int h;
};

} // namespace game

#endif // RECTANGLE_H
