#include "rectangle.h"
#include "log.h"
#include "display.h"
#include "rect.h"
#include "util_sdl.h"
#include "SDL.h"

namespace game {


Rectangle::Rectangle(int w, int h) :
  Graphics(),
  mColor(),
  w(w), h(h)
{
}

void Rectangle::draw(Display &target, int x, int y) const
{
  draw(target, Rect<int>(x, y, w, h));
}

void Rectangle::draw(Display &target, const Rect<int> &dest) const
{
  SDL_Rect renderRect = getSdlRect(dest);

  // Don't try to render if size is invalid
  if (renderRect.w <= 0 || renderRect.h <= 0)
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
