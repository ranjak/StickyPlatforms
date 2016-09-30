#include "groundstate.h"
#include "gamecommands.h"
#include "gamestate.h"
#include "airstate.h"
#include "movementcomponent.h"
#include "playerinputcomponent.h"
#include "walkcomponent.h"

namespace game {


GroundState::GroundState(PlayerInputComponent &stateMachine, WalkComponent &walkComp) :
  HorizControlState(stateMachine, walkComp, 2500.f)
{

}

void GroundState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  // Jump: set initial jump speed
  if (game.getCommands().isHit(Command::JUMP))
    mWalkComp.getMovement().velocity().y = - 1000.f;

  if (!game.getLevel().isOnGround(mStateMachine.getPlayer()))
    mStateMachine.setState(std::unique_ptr<AirState>(new AirState(mStateMachine, mWalkComp)));
}

}
