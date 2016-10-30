#include "entity.h"
#include "camera.h"
#include "graphics.h"
#include "log.h"
#include "gamestate.h"
#include "childremovedmsg.h"
#include "make_unique.h"
#include <algorithm>
#include <cassert>

namespace game {

const EntityID Entity::none = -1;

Entity::Entity(EntityID id, EntityManager &container, const Rect<float> &boundingBox, const std::string &name, EntityGroup group, EntityID parent) :
  id(id),
  group(group),
  mIsEnabled(true),
  mIsDead(false),
  mBoundingBox(boundingBox),
  mParent(Entity::none),
  mChildren(),
  mName(name),
  mComponents(),
  mContainer(container)
{
  if (parent != Entity::none) {
    Entity *parentPtr = mContainer.getEntity(parent);
    assert(parentPtr);
    parentPtr->addChild(*this);
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

void Entity::sendMessage(std::unique_ptr<Message> message)
{
  for (std::unique_ptr<Component> &comp : mComponents)
    comp->receiveMessage(*message);
}

void Entity::addComponent(std::unique_ptr<Component> component)
{
  mComponents.push_back(std::move(component));
}

void Entity::setEnabled(bool enabled)
{
  mIsEnabled = enabled;
}

void Entity::addChild(Entity &child)
{
  if (child.mParent != Entity::none)
    child.detach();

  child.mParent = id;
  mChildren.push_back(child.id);
}

void Entity::removeChild(EntityID child)
{
  Entity *childPtr = mContainer.getEntity(child);

  if (childPtr) {

    if (childPtr->mParent == id) {
      childPtr->mParent = Entity::none;
      sendMessage(std::make_unique<ChildRemovedMsg>(child));
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

Entity *Entity::getParent()
{
  return (mParent != Entity::none) ? mContainer.getEntity(mParent) : nullptr;
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
