#include "jumpstate.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "inputcomponent.h"
#include "game.h"
#include "log.h"
#include <cmath>
#include <cassert>

namespace game {

JumpState::JumpState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed, float friction, float maxJumpHeight, float impulseTimeFrame) :
  AirClingableState(stateMachine, maxSpeed, maxAirSpeed, friction),
  mMaxJumpHeight(maxJumpHeight),
  mImpulseTimeFrame(impulseTimeFrame),
  mInitialSpeed(computeInitialSpeed())
{
  assert(impulseTimeFrame >= 0.f);
}

void JumpState::enter()
{
  AirClingableState::enter();

  mImpulseEndTimestamp = Game::current().now() + static_cast<std::uint32_t>(mImpulseTimeFrame * 1000.f);

  mStateMachine.physics().velocity().y = mInitialSpeed;
  mStateMachine.physics().setGravityEnabled(false);
}

void JumpState::update(uint32_t step, Game &game)
{
  AirClingableState::update(step, game);

  if (game.now() > mImpulseEndTimestamp || mStateMachine.input().isReleased(Command::JUMP)) {
    mStateMachine.setState(ActorControlComponent::AIR_CLINGABLE);
  }
}

void JumpState::exit()
{
  mStateMachine.physics().setGravityEnabled(true);
}

float JumpState::computeInitialSpeed()
{
  float g = MovingPhysicsComponent::GRAVITY;

  // This computation is based on a polynom describing maximum height reached
  // as a function of impulse speed, taking into account gravity
  // and impulse time (time during which gravity is turned off).

  float discriminant = mImpulseTimeFrame*mImpulseTimeFrame - (2.f*(-mMaxJumpHeight))/g;

  return (-mImpulseTimeFrame + std::sqrt(discriminant)) / (-1.f/g);
}

} // namespace game
