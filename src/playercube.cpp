#include "playercube.h"
#include "log.h"

namespace game {


PlayerCube::PlayerCube() :
  PlayerCube(0, 0, 0, 0)

{

}

PlayerCube::PlayerCube(int x, int y, int w, int h) :
  Graphics(),
  sizePos{x, y, w, h},
  r(255), g(255), b(255), a(255)
{
}

void PlayerCube::draw(Display &target) const
{
  SDL_Renderer* renderer = target.getRenderer();

  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, &sizePos);
}

void PlayerCube::setPos(int x, int y)
{
  this->sizePos.x = x;
  this->sizePos.y = y;
}

void PlayerCube::setSize(int w, int h)
{
  this->sizePos.w = w;
  this->sizePos.h = h;
}


} // namespace game
