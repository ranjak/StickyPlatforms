#include "staticphysicscomponent.h"
#include "entitymanager.h"
#include "physicsmanager.h"

namespace game {

StaticPhysicsComponent::StaticPhysicsComponent(Entity &owner) :
  PhysicsComponent(owner)
{
  owner.manager().getPhysics().addComponent(this);
}

StaticPhysicsComponent::~StaticPhysicsComponent()
{
  mEntity.manager().getPhysics().removeComponent(this);
}

} // namespace game
