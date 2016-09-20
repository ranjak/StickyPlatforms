#include "entity.h"
#include "camera.h"
#include "graphics.h"

namespace game {


Entity::Entity() :
  Entity(0, 0, 0, 0)
{

}

Entity::Entity(int x, int y, int w, int h, std::unique_ptr<Graphics> graphs, Entity *parent, bool isObstacle) :
  mBoundingBox(x, y, w, h),
  mGraphics(std::move(graphs)),
  mParent(parent),
  mIsObstacle(isObstacle),
  mIsCollidable(true),
  mIgnoresObstacles(false)
{

}

void Entity::update(uint32_t step, GameState &game)
{

}

void Entity::draw(Display &target, const Camera &camera) const
{
  if (mGraphics) {
    Vector<float> pos = camera.toCamCoords(getGlobalPos());
    mGraphics->draw(target, pos.x, pos.y);
  }
}

void Entity::onObstacleReached(const Vector<int> &normal)
{

}

void Entity::onCollision(Entity &entity)
{

}

void Entity::onCollision(Tile &tile, Vector<int> pos)
{

}

bool Entity::isObstacle() const
{
  return mIsObstacle;
}

bool Entity::isCollidable() const
{
  return mIsCollidable;
}

bool Entity::ignoresObstacles() const
{
  return mIgnoresObstacles;
}

bool Entity::isDead() const
{
  return false;
}

Vector<float> Entity::getLocalPos() const
{
  return Vector<float>(mBoundingBox.x, mBoundingBox.y);
}

Vector<float> Entity::getGlobalPos() const
{
  if (mParent) {
    Vector<float> ppos = mParent->getGlobalPos();
    ppos.x += mBoundingBox.x;
    ppos.y += mBoundingBox.y;
    return ppos;
  }

  return Vector<float>(mBoundingBox.x, mBoundingBox.y);
}

void Entity::setLocalPos(const Vector<float> &newPos)
{
  mBoundingBox.x = newPos.x;
  mBoundingBox.y = newPos.y;
}

void Entity::setGlobalPos(const Vector<float> &newPos)
{
  if (mParent) {
    Vector<float> ppos = mParent->getGlobalPos();
    mBoundingBox.x = newPos.x - ppos.x;
    mBoundingBox.y = newPos.y - ppos.y;
  }
  else {
    mBoundingBox.x = newPos.x;
    mBoundingBox.y = newPos.y;
  }
}

Rect<float> &Entity::getLocalBox()
{
  return mBoundingBox;
}

const Rect<float> &Entity::getLocalBox() const
{
  return mBoundingBox;
}

Rect<float> Entity::getGlobalBox() const
{
  if (mParent) {
    Rect<float> parentBox = mParent->getGlobalBox();
    parentBox.x += mBoundingBox.x;
    parentBox.y += mBoundingBox.y;
    parentBox.w = mBoundingBox.w;
    parentBox.h = mBoundingBox.h;

    return parentBox;
  }

  return mBoundingBox;
}

} // namespace game
