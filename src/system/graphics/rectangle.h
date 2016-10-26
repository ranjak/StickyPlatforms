#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "graphics.h"
#include "color.h"
#include "gamevector.h"
#include <cstdint>

namespace game {


class Rectangle : public Graphics
{
public:
  using Graphics::draw;

  Rectangle(int w=0, int h=0, const Color &color=Color::WHITE);

  void draw(Display& target, int x, int y) const override;

  void draw(Display &target, const Rect<int> &dest) const override;

  void setSize(int w, int h);
  void setColor(const Color& mColor);

  Vector<float> getSize() { return Vector<float>(w, h); }

private:
  // Fill color of the rectangle
  Color mColor;
  // Size in window coordinates
  int w;
  int h;
};

} // namespace game

#endif // RECTANGLE_H
