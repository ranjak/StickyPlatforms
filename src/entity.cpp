#include "entity.h"

namespace game {


Entity::Entity() :
  Entity(0, 0, 0, 0)
{

}

Entity::Entity(int x, int y, int w, int h) :
  mBoundingBox(x, y, w, h)
{

}

void Entity::onObstacleReached(const Vector<int> &normal)
{

}

Vector<int> Entity::getPosition() const
{
  return Vector<int>(mBoundingBox.x, mBoundingBox.y);
}

void Entity::setPosition(const Vector<int> &newPos)
{
  mBoundingBox.x = newPos.x;
  mBoundingBox.y = newPos.y;
}

Rect<float> &Entity::getBoundingBox()
{
  return mBoundingBox;
}

const Rect<float> &Entity::getBoundingBox() const
{
  return mBoundingBox;
}

} // namespace game
