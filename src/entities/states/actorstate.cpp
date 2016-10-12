#include "actorstate.h"

namespace game {


ActorState::ActorState(ActorControlComponent& stateMachine) :
  mStateMachine(stateMachine)
{

}

void ActorState::update(std::uint32_t step, GameState &game)
{

}

void ActorState::enter()
{

}

void ActorState::receiveMessage(Message &msg)
{

}

int ActorState::getDirection()
{
  return 0;
}

}
