#include "actorstate.h"
#include "damagemsg.h"
#include "actorcontrolcomponent.h"

namespace game {


ActorState::ActorState(ActorControlComponent& stateMachine) :
  mStateMachine(stateMachine)
{

}

void ActorState::update(std::uint32_t step, Game &game)
{

}

void ActorState::enter()
{

}

void ActorState::exit()
{

}

void ActorState::receiveMessage(Message &msg)
{
  // Briefly remove control from the actor when hurt
  if (msg.type == Message::Damage) {
    mStateMachine.setState(ActorControlComponent::FALL);
  }
}

}
