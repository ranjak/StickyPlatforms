#ifndef STATICPHYSICSCOMPONENT_H
#define STATICPHYSICSCOMPONENT_H

#include "physicscomponent.h"

namespace game {


class StaticPhysicsComponent : public PhysicsComponent
{
public:
  StaticPhysicsComponent(Entity &owner);

  ~StaticPhysicsComponent();
};

} // namespace game

#endif // STATICPHYSICSCOMPONENT_H
