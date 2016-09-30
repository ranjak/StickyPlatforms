#include "airstate.h"
#include "gamevector.h"
#include "hero.h"
#include "groundstate.h"
#include "gamestate.h"
#include "playerinputcomponent.h"

namespace game {

AirState::AirState(PlayerInputComponent& stateMachine, WalkComponent &walkComp) :
    HorizControlState(stateMachine, walkComp, 1200.f)
{

}

void AirState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  if (game.getLevel().isOnGround(mStateMachine.getPlayer()))
    mStateMachine.setState(std::unique_ptr<GroundState>(new GroundState(mStateMachine, mWalkComp)));
}

}
