#include "tile.h"
#include "camera.h"
#include "gamevector.h"
#include "rect.h"
#include "image.h"

namespace game {


const int Tile::SIZE = 32;

Tile::Tile(TileID id,
           bool isObstacle,
           const std::shared_ptr<Image> &graphics,
           const Rect<int> &graphicsPos,
           const std::function<void(Entity &, Level &)> &collisionBehavior)
  :
    id(id),
    mIsObstacle(isObstacle),
    mGraphics(graphics),
    mCollisionBehavior(collisionBehavior),
    mGraphicsPos(graphicsPos)
{
  // Unspecified position in the image ? Take the whole image
  if (mGraphicsPos.w <= 0 && mGraphics) {
    mGraphicsPos.w = mGraphics->size().x;
    mGraphicsPos.h = mGraphics->size().y;
  }
}

bool Tile::isObstacle() const
{
  return mIsObstacle;
}

void Tile::draw(Display& display, int x, int y, const Camera &cam) const
{
  if (mGraphics) {
    mGraphics->draw(display, cam.toCamCoords(Rect<int>(x*SIZE, y*SIZE, SIZE, SIZE)), mGraphicsPos);
  }
}

void Tile::onCollision(Entity &entity, Level &level)
{
  if (mCollisionBehavior)
    mCollisionBehavior(entity, level);
}

}
