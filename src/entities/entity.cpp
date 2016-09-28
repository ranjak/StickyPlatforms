#include "entity.h"
#include "camera.h"
#include "graphics.h"
#include "log.h"
#include <algorithm>

namespace game {


Entity::Entity() :
  Entity(0, 0, 0, 0)
{

}

Entity::Entity(int x, int y, int w, int h, bool isObstacle, const std::string &name, std::unique_ptr<Graphics> graphs, Entity *parent) :
  mBoundingBox(x, y, w, h),
  mGraphics(std::move(graphs)),
  mParent(),
  mChildren(),
  mIsObstacle(isObstacle),
  mIsCollidable(true),
  mIgnoresObstacles(false),
  mName(name)
{
  if (parent != nullptr)
    parent->addChild(this);
}

Entity::~Entity()
{
  // Notify related entities
  for (Entity *child : mChildren) {
    child->mParent = nullptr;
  }

  if (mParent != nullptr)
    mParent->removeChild(this);
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

void Entity::addChild(Entity *child)
{
  if (child->mParent)
    child->mParent->removeChild(child);

  child->mParent = this;
  mChildren.push_back(child);
}

void Entity::removeChild(Entity *child)
{
  if (child->mParent == this) {
    child->mParent = nullptr;
    mChildren.erase(std::find(mChildren.begin(), mChildren.end(), child));
  }
  else {
    Log::getGlobal().get(Log::WARNING) << "Entity " << child << " doesn't have " << this << " as parent, can't remove from children list";
  }
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
