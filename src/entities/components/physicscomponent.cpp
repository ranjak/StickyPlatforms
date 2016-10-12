#include "physicscomponent.h"
#include "gamevector.h"
#include "make_unique.h"
#include "rect.h"
#include "collisionmsg.h"

namespace game {

PhysicsComponent::PhysicsComponent(Entity &owner, bool isObstacle) :
  mIsCollidable(true),
  mIsObstacle(isObstacle),
  mEntity(owner),
  mCollidingEntities()
{

}

void PhysicsComponent::update(uint32_t step, GameState &game)
{
  mCollidingEntities.clear();
}

void PhysicsComponent::collide(PhysicsComponent &other)
{
  mCollidingEntities.push_back(other.entity().id);

  Vector<int> normal = mEntity.getGlobalBox().getCollisionNormal(other.entity().getGlobalBox());

  mEntity.sendMessage(std::make_unique<CollisionMsg>(other.entity().id, normal, other.isObstacle()));
}

} // namespace game
