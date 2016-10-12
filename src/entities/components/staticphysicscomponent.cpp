#include "staticphysicscomponent.h"
#include "entitymanager.h"
#include "physicsmanager.h"

namespace game {

StaticPhysicsComponent::StaticPhysicsComponent(Entity &owner, bool isObstacle) :
  PhysicsComponent(owner, isObstacle)
{
  owner.manager().getPhysics().addComponent(this);
}

StaticPhysicsComponent::~StaticPhysicsComponent()
{
  mEntity.manager().getPhysics().removeComponent(this);
}

} // namespace game
