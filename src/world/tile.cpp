#include "tile.h"
#include "graphics.h"

namespace game {


const int Tile::SIZE = 32;

Tile::Tile(bool isObstacle, const std::shared_ptr<Graphics> &graphics) :
  mIsObstacle(isObstacle), mGraphics(graphics)
{

}

bool Tile::isObstacle() const
{
  return mIsObstacle;
}

void Tile::draw(Display& display, int x, int y) const
{
  if (mGraphics.get() != nullptr)
    mGraphics->draw(display, x*SIZE, y*SIZE);
}

}
