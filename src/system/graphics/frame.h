#ifndef FRAME_H
#define FRAME_H

#include "graphics.h"
#include "color.h"

namespace game {

class Frame : public Graphics
{
public:
  using Graphics::draw;

  Frame(int w=0, int h=0, const Color &color=Color::WHITE);

  void draw(Display &target, int x, int y) const override;
  void draw(Display &target, const Rect<int> &dest) const override;

  void setColor(const Color &color) { mColor = color; }

private:
  // Fill color of the rectangle
  Color mColor;
  // Size in window coordinates
  int w;
  int h;
};

} // namespace game

#endif // FRAME_H
