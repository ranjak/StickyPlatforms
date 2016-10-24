#include "fallstate.h"
#include "gamestate.h"
#include "actorcontrolcomponent.h"

namespace game {

FallState::FallState(ActorControlComponent &stateMachine) :
  ActorState(stateMachine)
{

}

void FallState::enter()
{
  mEndTimestamp = GameState::current().now() + 500;
}

void FallState::update(uint32_t step, GameState &game)
{
  if (game.now() > mEndTimestamp)
    mStateMachine.setState(ActorControlComponent::AIR);
}

} // namespace game
