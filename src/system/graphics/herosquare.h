#ifndef HEROSQUARE_H
#define HEROSQUARE_H

#include "rectangle.h"
#include "color.h"
#include "gamevector.h"

namespace game {

class HeroSquare : public Rectangle
{
public:
  HeroSquare(int w=32, int h=32, const Color &color=Color::GREEN, const Color &eyeColor=Color::WHITE);

  void draw(Display &target, const Rect<int> &dest) override;

  void setDirection(int direction);

private:
  Rectangle mEye;
  int mDirection;
  Vector<int> mEyePos;
};

} // namespace game

#endif // HEROSQUARE_H
