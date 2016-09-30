#include "physicscomponent.h"
#include "movementcomponent.h"
#include "obstaclereachedmsg.h"
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

void PhysicsComponent::receiveMessage(Message &message)
{
  switch (message.type) {

  case Message::ObstacleReached:
  {
    const Vector<int> &normal = static_cast<ObstacleReachedMsg &>(message).normal;
    Vector<float> &velocity = mMovement.velocity();

    if (normal.x != 0)
      velocity.x = 0.f;

    if (normal.y != 0)
      velocity.y = 0.f;

    break;
  }
  default:
    break;
  }
}

} // namespace game
