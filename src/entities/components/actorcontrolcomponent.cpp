#include "actorcontrolcomponent.h"
#include "weaponcomponent.h"
#include "airstate.h"
#include "make_unique.h"
#include "log.h"
#include "entity.h"

namespace game {


ActorControlComponent::ActorControlComponent(Entity &entity, MovingPhysicsComponent &physics, InputComponent &input, float maxSpeed, float maxAirSpeed) :
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
  mClimbState(*this)
{

}

void ActorControlComponent::update(uint32_t step, GameState &game)
{
  if (mNextState != NONE) {

    switch (mNextState) {
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
    default:
      game::error("Entity "+mEntity.getName()+": unknown state: "+std::to_string(mNextState));
    }

    mCurrentState->enter();
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
  mNextState = newState;

  if (mCurrentState)
    mCurrentState->exit();
}


} // namespace game
