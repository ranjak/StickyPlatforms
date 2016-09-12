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

void Entity::update(uint32_t step, GameState &game)
{

}

void Entity::draw(Display &target, const Camera &camera) const
{

}

void Entity::onObstacleReached(const Vector<int> &normal)
{

}

Vector<float> Entity::getPosition() const
{
  return Vector<float>(mBoundingBox.x, mBoundingBox.y);
}

void Entity::setPosition(const Vector<float> &newPos)
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
