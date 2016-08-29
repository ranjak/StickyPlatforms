#include "rectangle.h"
#include "log.h"

namespace game {


Rectangle::Rectangle() :
  Rectangle(0, 0, 0, 0)

{

}

Rectangle::Rectangle(int x, int y, int w, int h) :
  Graphics(),
  sizePos{x, y, w, h},
  color()
{
}

void Rectangle::draw(Display &target) const
{
  // Don't try to render if size is invalid
  if (sizePos.w <= 0 || sizePos.h <= 0)
    return;

  SDL_Renderer* renderer = target.getRenderer();

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &sizePos);
}

void Rectangle::setPos(int x, int y)
{
  this->sizePos.x = x;
  this->sizePos.y = y;
}

void Rectangle::setSize(int w, int h)
{
  this->sizePos.w = w;
  this->sizePos.h = h;
}

void Rectangle::setColor(const Color &color)
{
  this->color = color;
}


} // namespace game
