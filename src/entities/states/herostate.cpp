#include "herostate.h"

namespace game {


HeroState::HeroState(PlayerInputComponent& stateMachine) :
  mStateMachine(stateMachine)
{

}

void HeroState::update(std::uint32_t step, GameState &game)
{

}

void HeroState::enter()
{

}

void HeroState::receiveMessage(Message &msg)
{

}

}
