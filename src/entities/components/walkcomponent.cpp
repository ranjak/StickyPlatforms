#include "walkcomponent.h"
#include "gamevector.h"
#include "movementcomponent.h"
#include "directionchangedmsg.h"
#include <algorithm>


namespace game {

WalkComponent::WalkComponent(MovementComponent &movement) :
  mMovement(movement),
  mAcceleration(0),
  mMaxSpeed(0),
  mWalkDirection(0)
{

}

void WalkComponent::update(uint32_t step, GameState &game)
{
  Vector<float>& velocity = mMovement.velocity();

  float accelAmount = mAcceleration * step / 1000.f;

  // Accelerate until max speed
  if (mWalkDirection < 0)
    velocity.x = std::max(-mMaxSpeed, velocity.x - accelAmount);

  else if (mWalkDirection > 0)
    velocity.x = std::min(mMaxSpeed, velocity.x + accelAmount);

  // No direction : decelerate until stop
  else if (velocity.x > 0)
    velocity.x = std::max(0.f, velocity.x - accelAmount);
  else if (velocity.x < 0)
    velocity.x = std::min(0.f, velocity.x + accelAmount);
}

void WalkComponent::receiveMessage(Message &msg)
{
  switch (msg.type) {

  case Message::DirectionChanged:
    mWalkDirection = static_cast<DirectionChangedMsg &>(msg).direction;
    break;

  default:
    break;
  }
}

void WalkComponent::setDirection(int direction)
{
  mWalkDirection = direction;
}

void WalkComponent::setAcceleration(float acceleration)
{
  mAcceleration = acceleration;
}

void WalkComponent::setMaxSpeed(float maxSpeed)
{
  mMaxSpeed = maxSpeed;
}

} // namespace game
