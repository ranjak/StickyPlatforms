#include "herosquare.h"
#include "rect.h"

namespace game {

HeroSquare::HeroSquare(int w, int h, const Color &color, const Color &eyeColor) :
  Rectangle(w, h, color),
  mEye(2, 2, eyeColor),
  mDirection(1),
  mEyePos(w-2-8, 10)
{

}

void HeroSquare::draw(Display &target, const Rect<int> &dest)
{
  Rectangle::draw(target, dest);

  mEye.draw(target, dest.x + mEyePos.x, dest.y + mEyePos.y);
}

void HeroSquare::setDirection(int direction)
{
  // Direction shouldn't be 0, don't change if it is
  if (direction == 0)
    return;

  mDirection = direction;
  mEyePos.x = (direction < 0) ? 8 : getSize().x-mEye.getSize().x-8;
}

} // namespace game
