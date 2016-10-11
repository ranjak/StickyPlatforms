#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "component.h"
#include "entity.h"
#include <vector>

namespace game {


class PhysicsComponent : public Component
{
public:
  bool isObstacle() { return mIsObstacle; }

  Entity &entity() { return mEntity; }

  void collide(PhysicsComponent &other);

protected:
  PhysicsComponent(Entity &owner);

protected:
  bool mIsObstacle;
  Entity &mEntity;
  std::vector<EntityID> mCollidingEntities;
};

} // namespace game

#endif // PHYSICSCOMPONENT_H
