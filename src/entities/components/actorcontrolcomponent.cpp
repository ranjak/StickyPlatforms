#include "actorcontrolcomponent.h"
#include "weaponcomponent.h"
#include "make_unique.h"
#include "log.h"
#include "entity.h"
#include "movingphysicscomponent.h"
#include <cassert>

namespace game {


ActorControlComponent::ActorControlComponent(Entity &entity, MovingPhysicsComponent &physics, InputComponent &input, float maxSpeed, float maxAirSpeed) :
  mMaxSpeed(maxSpeed),
  mCurrentState(),
  mNextState(AIR),
  mInput(input),
  mPhysics(physics),
  mEntity(entity),
  mGroundState(*this, maxSpeed),
  mAirState(*this, maxSpeed, maxAirSpeed, 1600.f),
  mAirClingableState(*this, maxSpeed, maxAirSpeed, 200.f),
  mJumpState(*this, maxSpeed, maxAirSpeed, 200.f),
  mClingState(*this),
  mClimbState(*this),
  mClimbPlatformState(*this),
  mFallState(*this)
{

}

void ActorControlComponent::updateDelegate(uint32_t step, GameState &game)
{
  // For initialization, and states that might need a delayed call to enter()
  if (mNextState != NONE) {
    setState(mNextState);
    mNextState = NONE;
  }

  mCurrentState->update(step, game);
}

void ActorControlComponent::receiveMessageDelegate(Message &msg)
{
  mCurrentState->receiveMessage(msg);
}

void ActorControlComponent::setState(State newState)
{
  if (mCurrentState)
    mCurrentState->exit();

  switch (newState) {
  case AIR:
    mCurrentState = &mAirState;
    break;
  case AIR_CLINGABLE:
    mCurrentState = &mAirClingableState;
    break;
  case JUMP:
    mCurrentState = &mJumpState;
    break;
  case GROUND:
    mCurrentState = &mGroundState;
    break;
  case CLING:
    mCurrentState = &mClingState;
    break;
  case CLIMB:
    mCurrentState = &mClimbState;
    break;
  case CLIMB_PLATFORM:
    mCurrentState = &mClimbPlatformState;
    break;
  case FALL:
    mCurrentState = &mFallState;
    break;
  default:
    game::error("Entity "+mEntity.getName()+": unknown state: "+std::to_string(newState));
  }

  glog(Log::DBG, "setState: " << mEntity << " switching to state " << newState);

  mCurrentState->enter();
}

int ActorControlComponent::getDirection() const
{
  // Direction set by the state?
  if (mDirection != 0)
    return mDirection;

  // Movement direction? If none, face right by default.
  return (mPhysics.velocity().x < 0.f) ? -1 : 1;
}

void ActorControlComponent::setDirection(int direction)
{
  assert(-1 <= direction && direction <= 1);
  mDirection = direction;
}

float ActorControlComponent::getMaxJumpHeight() const
{
  return mJumpState.getMaxJumpHeight();
}


} // namespace game
