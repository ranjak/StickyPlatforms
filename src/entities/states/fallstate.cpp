#include "fallstate.h"
#include "game.h"
#include "actorcontrolcomponent.h"

namespace game {

FallState::FallState(ActorControlComponent &stateMachine) :
  ActorState(stateMachine)
{

}

void FallState::enter()
{
  mEndTimestamp = Game::current().now() + 250;
}

void FallState::update(uint32_t step, Game &game)
{
  if (game.now() > mEndTimestamp)
    mStateMachine.setState(ActorControlComponent::AIR);
}

} // namespace game
