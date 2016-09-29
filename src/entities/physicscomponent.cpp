#include "physicscomponent.h"
#include "movementcomponent.h"
#include "gamevector.h"

namespace game {

PhysicsComponent::PhysicsComponent(MovementComponent &movement) :
  mGravity(2500.f),
  mFallSpeed(800.f),
  mMovement(movement)
{

}

void PhysicsComponent::update(uint32_t step, GameState &game)
{
  // Apply gravity (accelerate until max fall speed)
  Vector<float> &velocity = mMovement.velocity();

  if (velocity.y < mFallSpeed)
    velocity.y += mGravity * step / 1000.f;
  else
    velocity.y = mFallSpeed;
}

} // namespace game
