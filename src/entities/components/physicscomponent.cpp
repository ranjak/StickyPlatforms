#include "physicscomponent.h"
#include "gamevector.h"
#include "make_unique.h"
#include "rect.h"
#include "collision.h"

namespace game {

PhysicsComponent::PhysicsComponent(Entity &owner, bool isObstacle) :
  mIsCollidable(true),
  mIsObstacle(isObstacle),
  mEntity(owner),
  mCollisions()
{

}

void PhysicsComponent::update(uint32_t step, GameState &game)
{
  mCollisions.clear();
}

void PhysicsComponent::collide(PhysicsComponent &other)
{
  Vector<int> normal = mEntity.getGlobalBox().getCollisionNormal(other.entity().getGlobalBox());

  auto it = mCollisions.emplace(mCollisions.end(), other.entity().id, other.entity().getGlobalBox(), normal, other.isObstacle());

  mEntity.sendMessage(std::make_unique<Collision>(*it));
}

} // namespace game
