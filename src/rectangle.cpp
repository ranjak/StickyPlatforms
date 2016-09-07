#include "rectangle.h"
#include "log.h"

namespace game {


Rectangle::Rectangle() :
  Rectangle(0, 0, 0, 0)

{

}

Rectangle::Rectangle(int x, int y, int w, int h) :
  Graphics(),
  mSizePos{x, y, w, h},
  mColor()
{
}

void Rectangle::draw(Display &target) const
{
  // Don't try to render if size is invalid
  if (mSizePos.w <= 0 || mSizePos.h <= 0)
    return;

  SDL_Renderer* renderer = target.getRenderer();

  SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
  SDL_RenderFillRect(renderer, &mSizePos);
}

void Rectangle::setPos(int x, int y)
{
  this->mSizePos.x = x;
  this->mSizePos.y = y;
}

void Rectangle::setSize(int w, int h)
{
  this->mSizePos.w = w;
  this->mSizePos.h = h;
}

void Rectangle::setColor(const Color &color)
{
  this->mColor = color;
}


} // namespace game
