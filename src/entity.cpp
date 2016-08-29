#include "entity.h"

namespace game {


Entity::Entity() :
  pos()
{

}

Entity::Entity(int x, int y) :
  pos(x, y)
{

}

Vector<int> Entity::getPosition() const
{
  return pos;
}

void Entity::setPosition(const Vector<int> &newPos)
{
  pos = newPos;
}

} // namespace game
