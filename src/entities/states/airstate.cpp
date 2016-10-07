#include "airstate.h"
#include "gamevector.h"
#include "hero.h"
#include "groundstate.h"
#include "gamestate.h"
#include "actorcontrolcomponent.h"
#include "physicscomponent.h"
#include "make_unique.h"

namespace game {

AirState::AirState(ActorControlComponent &stateMachine) :
    HorizControlState(stateMachine, 1200.f)
{

}

void AirState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  if (mStateMachine.physics().isOnGround())
    mStateMachine.setState(std::make_unique<GroundState>(mStateMachine));
}

}
