#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "component.h"
#include "entity.h"
#include <vector>

namespace game {


class PhysicsComponent : public Component
{
public:

  void update(uint32_t step, GameState &game);

  bool isObstacle() { return mIsObstacle; }

  bool isCollidable() { return mEntity.isEnabled() && mIsCollidable; }

  void setCollidable(bool collidable) { mIsCollidable = collidable; }

  Entity &entity() { return mEntity; }

  void collide(PhysicsComponent &other);

protected:
  PhysicsComponent(Entity &owner, bool isObstacle=true);

protected:
  bool mIsCollidable;
  bool mIsObstacle;
  Entity &mEntity;
  std::vector<EntityID> mCollidingEntities;
};

} // namespace game

#endif // PHYSICSCOMPONENT_H
