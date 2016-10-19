#include "jumpstate.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "inputcomponent.h"
#include "gamestate.h"
#include "log.h"
#include <cmath>

namespace game {

JumpState::JumpState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed, float maxJumpHeight, float impulseTimeFrame) :
  AirClingableState(stateMachine, maxSpeed, maxAirSpeed),
  mMaxJumpHeight(maxJumpHeight),
  mImpulseTimeFrame(impulseTimeFrame),
  mInitialSpeed(computeInitialSpeed())
{
}

void JumpState::enter()
{
  mImpulseEndTimestamp = GameState::current().now() + (mImpulseTimeFrame * 1000);

  mStateMachine.physics().velocity().y = mInitialSpeed;
  mStateMachine.physics().setGravityEnabled(false);
}

void JumpState::update(uint32_t step, GameState &game)
{
  // If we don't take off immediately, the parent could put us back in GroundState
  if (!mStateMachine.physics().isOnGround()) {
    AirClingableState::update(step, game);
  }

  if (game.now() >= mImpulseEndTimestamp || mStateMachine.input().isReleased(Command::JUMP)) {
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

  float discriminant = mImpulseTimeFrame*mImpulseTimeFrame - (2.f*(-mMaxJumpHeight))/g;

  return (-mImpulseTimeFrame + std::sqrt(discriminant)) / (-1.f/g);
}

} // namespace game
