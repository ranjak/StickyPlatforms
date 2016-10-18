#include "actorcontrolcomponent.h"
#include "weaponcomponent.h"
#include "airstate.h"
#include "make_unique.h"
#include "log.h"
#include "entity.h"

namespace game {


ActorControlComponent::ActorControlComponent(Entity &entity, MovingPhysicsComponent &physics, InputComponent &input, float maxSpeed) :
  mCurrentState(),
  mNextState(AIR),
  mInput(input),
  mPhysics(physics),
  mEntity(entity),
  mGroundState(*this, maxSpeed),
  mAirState(*this, maxSpeed),
  mAirClingableState(*this, maxSpeed),
  mClingState(*this)
{

}

void ActorControlComponent::update(uint32_t step, GameState &game)
{
  if (mNextState != NONE) {

    if (mCurrentState)
      mCurrentState->exit();

    switch (mNextState) {
    case AIR:
      mCurrentState = &mAirState;
      break;
    case AIR_CLINGABLE:
      mCurrentState = &mAirClingableState;
      break;
    case GROUND:
      mCurrentState = &mGroundState;
      break;
    case CLING:
      mCurrentState = &mClingState;
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
}


} // namespace game
