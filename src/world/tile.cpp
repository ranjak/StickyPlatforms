#include "tile.h"
#include "camera.h"
#include "gamevector.h"
#include "rect.h"
#include "image.h"
#include "log.h"

namespace game {


const int Tile::SIZE = 32;

Tile::Tile(TileID id,
           bool isObstacle,
           const std::shared_ptr<Image> &graphics,
           const Rect<int> &graphicsPos, const Rect<float> &collisionBox,
           const std::function<void(Entity &, Level &)> &collisionBehavior)
  :
    id(id),
    mIsObstacle(isObstacle),
    mGraphics(graphics),
    mCollisionBehavior(collisionBehavior),
    mGraphicsPos(graphicsPos),
    mCollisionBox(collisionBox)
{
  // Unspecified position in the image ? Take the whole image
  if (mGraphicsPos.w <= 0 && mGraphics) {
    mGraphicsPos.w = mGraphics->size().x;
    mGraphicsPos.h = mGraphics->size().y;
  }

  glog(Log::DBG, "New Tile: id=" <<id<<",isObstacle="<<isObstacle<<",graphics="<<graphics<<",graphicsPos="<<graphicsPos<<",collisionBox="<<collisionBox);
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

Rect<float> Tile::getCollisionBox(int x, int y) const
{
  return Rect<float>(x*SIZE + mCollisionBox.x, y*SIZE + mCollisionBox.y, mCollisionBox.w, mCollisionBox.h);
}

void Tile::onCollision(Entity &entity, Level &level)
{
  if (mCollisionBehavior)
    mCollisionBehavior(entity, level);
}

}
