#include "rectangle.h"
#include "log.h"

namespace game {


Rectangle::Rectangle(int w, int h) :
  Graphics(),
  w(w), h(h),
  mColor()
{
}

void Rectangle::draw(Display &target, int x, int y) const
{
  SDL_Rect renderRect { x, y, w, h };

  // Don't try to render if size is invalid
  if (w <= 0 || h <= 0)
    return;

  SDL_Renderer* renderer = target.getRenderer();

  SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
  SDL_RenderFillRect(renderer, &renderRect);
}

void Rectangle::setSize(int w, int h)
{
  this->w = w;
  this->h = h;
}

void Rectangle::setColor(const Color &color)
{
  mColor = color;
}


} // namespace game
