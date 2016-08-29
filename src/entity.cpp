#include "entity.h"

namespace game {


Entity::Entity() :
  boundingBox()
{

}

Entity::Entity(int x, int y, int w, int h) :
  boundingBox(x, y, w, h)
{

}

Vector<int> Entity::getPosition() const
{
  return Vector<int>(boundingBox.x, boundingBox.y);
}

void Entity::setPosition(const Vector<int> &newPos)
{
  boundingBox.x = newPos.x;
  boundingBox.y = newPos.y;
}

const Rect &Entity::getBoundingBox()
{
  return boundingBox;
}

} // namespace game
