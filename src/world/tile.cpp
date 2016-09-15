#include "tile.h"
#include "graphics.h"
#include "camera.h"
#include "gamevector.h"
#include "rect.h"

namespace game {


const int Tile::SIZE = 32;

Tile::Tile(bool isObstacle,
           const std::shared_ptr<Graphics> &graphics,
           const std::function<void(Entity &, Level &)> &collisionBehavior)
  :
    mIsObstacle(isObstacle),
    mGraphics(graphics),
    mCollisionBehavior(collisionBehavior)
{

}

bool Tile::isObstacle() const
{
  return mIsObstacle;
}

void Tile::draw(Display& display, int x, int y, const Camera &cam) const
{
  if (mGraphics.get() != nullptr)
  {
    Vector<float> camCoords = cam.toCamCoords(Vector<float>(x*SIZE, y*SIZE));

    mGraphics->draw(display, Rect<int>((int)camCoords.x, (int)camCoords.y, SIZE, SIZE));
  }
}

void Tile::onCollision(Entity &entity, Level &level)
{
  if (mCollisionBehavior)
    mCollisionBehavior(entity, level);
}

}
