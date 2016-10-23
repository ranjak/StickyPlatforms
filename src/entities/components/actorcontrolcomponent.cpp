#include "actorcontrolcomponent.h"
#include "weaponcomponent.h"
#include "airstate.h"
#include "make_unique.h"
#include "log.h"
#include "entity.h"

namespace game {


ActorControlComponent::ActorControlComponent(Entity &entity, MovingPhysicsComponent &physics, InputComponent &input, float maxSpeed, float maxAirSpeed) :
  mMaxSpeed(maxSpeed),
  mCurrentState(),
  mNextState(AIR),
  mInput(input),
  mPhysics(physics),
  mEntity(entity),
  mGroundState(*this, maxSpeed),
  mAirState(*this, maxSpeed, maxAirSpeed),
  mAirClingableState(*this, maxSpeed, maxAirSpeed),
  mJumpState(*this, maxSpeed, maxAirSpeed),
  mClingState(*this),
  mClimbState(*this),
  mClimbPlatformState(*this)
{

}

void ActorControlComponent::update(uint32_t step, GameState &game)
{
  // For initialization, and states that might need a delayed call to enter()
  if (mNextState != NONE) {
    setState(mNextState);
    mNextState = NONE;
  }

  mCurrentState->update(step, game);
}

void ActorControlComponent::receiveMessage(Message &msg)
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
  default:
    game::error("Entity "+mEntity.getName()+": unknown state: "+std::to_string(newState));
  }

  mCurrentState->enter();
}


} // namespace game
