#include "actorcontrolcomponent.h"
#include "airstate.h"
#include "make_unique.h"

namespace game {


ActorControlComponent::ActorControlComponent(Entity &entity, PhysicsComponent &physics, InputComponent &input) :
  mCurrentState(),
  mNextState(std::make_unique<AirState>(*this)),
  mInput(input),
  mPhysics(physics),
  mEntity(entity)
{

}

void ActorControlComponent::update(uint32_t step, GameState &game)
{
  if (mNextState) {
    mCurrentState = std::move(mNextState);
    mCurrentState->enter();
  }

  mCurrentState->update(step, game);
}

void ActorControlComponent::receiveMessage(Message &msg)
{
  mCurrentState->receiveMessage(msg);
}

void ActorControlComponent::setState(std::unique_ptr<ActorState> newState)
{
  mNextState = std::move(newState);
}


} // namespace game
