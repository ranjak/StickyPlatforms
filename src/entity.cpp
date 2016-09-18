#include "entity.h"
#include "camera.h"
#include "graphics.h"

namespace game {


Entity::Entity() :
  Entity(0, 0, 0, 0)
{

}

Entity::Entity(int x, int y, int w, int h, std::unique_ptr<Graphics> graphs) :
  mBoundingBox(x, y, w, h),
  mGraphics(std::move(graphs))
{

}

void Entity::update(uint32_t step, GameState &game)
{

}

void Entity::draw(Display &target, const Camera &camera) const
{
  if (mGraphics) {
    Vector<float> pos = camera.toCamCoords(getPosition());
    mGraphics->draw(target, pos.x, pos.y);
  }
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
