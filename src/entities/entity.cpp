#include "entity.h"
#include "camera.h"
#include "graphics.h"
#include "log.h"
#include "gamestate.h"
#include <algorithm>
#include <cassert>

namespace game {


Entity::Entity(EntityID id, EntityManager &container, int x, int y, int w, int h, bool isObstacle, const std::string &name, std::unique_ptr<Graphics> graphs, EntityID parent) :
  id(id),
  mBoundingBox(x, y, w, h),
  mGraphics(std::move(graphs)),
  mParent(Entity::none),
  mChildren(),
  mIsObstacle(isObstacle),
  mIsCollidable(true),
  mIgnoresObstacles(false),
  mName(name),
  mComponents(),
  mContainer(container)
{
  if (parent != Entity::none) {
    Entity *parentPtr = mContainer.getEntity(parent);
    assert(parentPtr);
    parentPtr->addChild(id);
  }
}

Entity::~Entity()
{
  // Notify related entities
  for (EntityID child : mChildren) {
    removeChild(child);
  }

  if (mParent != Entity::none)
    detach();
}

void Entity::update(uint32_t step, GameState &game)
{
  for (std::unique_ptr<Component> &comp : mComponents)
    comp->update(step, game);
}

void Entity::draw(Display &target, const Camera &camera) const
{
  if (mGraphics) {
    Vector<float> pos = camera.toCamCoords(getGlobalPos());
    mGraphics->draw(target, pos.x, pos.y);
  }
}

void Entity::sendMessage(std::unique_ptr<Message> message)
{
  for (std::unique_ptr<Component> &comp : mComponents)
    comp->receiveMessage(*message);
}

void Entity::addComponent(std::unique_ptr<Component> component)
{
  mComponents.push_back(std::move(component));
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

void Entity::addChild(EntityID child)
{
  Entity *childPtr = mContainer.getEntity(child);
  if (!childPtr) {
    Log::getGlobal().get(Log::WARNING) << *this << ": could not add child "<<child<<": no such entity"<<std::endl;
    return;
  }

  if (childPtr->mParent != Entity::none)
    childPtr->detach();

  childPtr->mParent = id;
  mChildren.push_back(child);
}

void Entity::removeChild(EntityID child)
{
  Entity *childPtr = mContainer.getEntity(child);

  if (childPtr) {

    if (childPtr->mParent == id) {
      childPtr->mParent = Entity::none;
    }
    else {
      Log::getGlobal().get(Log::WARNING) <<*this<<"::removeChild: "<<*childPtr<<" doesn't have "<<*this<<" as parent, can't remove from children list"<<std::endl;
    }
  }
  else {
    Log::getGlobal().get(Log::WARNING) << *this << "::removeChild: child "<<child<<" doesn't exist"<<std::endl;
  }

  mChildren.erase(std::find(mChildren.begin(), mChildren.end(), child));
}

void Entity::detach()
{
  if (mParent != Entity::none) {
    Entity *parentPtr = mContainer.getEntity(mParent);
    assert(parentPtr);
    parentPtr->removeChild(id);
  }
  else {
    Log::getGlobal().get(Log::WARNING) <<*this<<"::detach: can't detach orphan entity"<<std::endl;
  }
}

Vector<float> Entity::getLocalPos() const
{
  return Vector<float>(mBoundingBox.x, mBoundingBox.y);
}

Vector<float> Entity::getGlobalPos() const
{
  if (mParent != Entity::none) {
    Entity *parentPtr = mContainer.getEntity(mParent);
    assert(parentPtr);

    Vector<float> ppos = parentPtr->getGlobalPos();
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
  if (mParent != Entity::none) {
    Entity *parentPtr = mContainer.getEntity(mParent);
    assert(parentPtr);

    Vector<float> ppos = parentPtr->getGlobalPos();
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
  if (mParent != Entity::none) {
    Entity *parentPtr = mContainer.getEntity(mParent);
    assert(parentPtr);

    Rect<float> parentBox = parentPtr->getGlobalBox();
    parentBox.x += mBoundingBox.x;
    parentBox.y += mBoundingBox.y;
    parentBox.w = mBoundingBox.w;
    parentBox.h = mBoundingBox.h;

    return parentBox;
  }

  return mBoundingBox;
}

} // namespace game
