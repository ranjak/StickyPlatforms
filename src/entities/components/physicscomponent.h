#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "component.h"
#include "entity.h"
#include "collision.h"
#include <vector>

namespace game {


class PhysicsComponent : public Component
{
public:

  bool isObstacle() { return mIsObstacle; }

  bool isCollidable() { return mEntity.isEnabled() && mIsCollidable; }

  void setCollidable(bool collidable) { mIsCollidable = collidable; }

  Entity &entity() { return mEntity; }

  const std::vector<Collision> &getCollisions() { return mCollisions; }

  void collide(PhysicsComponent &other);

protected:
  PhysicsComponent(Entity &owner, bool isObstacle=true);

  void updateDelegate(uint32_t step, Game &game);

protected:
  bool mIsCollidable;
  bool mIsObstacle;
  Entity &mEntity;
  std::vector<Collision> mCollisions;
};

} // namespace game

#endif // PHYSICSCOMPONENT_H
