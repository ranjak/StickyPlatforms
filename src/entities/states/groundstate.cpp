#include "groundstate.h"
#include "gamecommands.h"
#include "airstate.h"
#include "actorcontrolcomponent.h"
#include "physicscomponent.h"
#include "inputcomponent.h"
#include "make_unique.h"

namespace game {


GroundState::GroundState(ActorControlComponent &stateMachine, float maxSpeed) :
  HorizControlState(stateMachine, 2500.f, maxSpeed)
{

}

void GroundState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  // Jump: set initial jump speed
  if (mStateMachine.input().isHit(Command::JUMP))
    mStateMachine.physics().velocity().y = - 1000.f;

  if (!mStateMachine.physics().isOnGround())
    mStateMachine.setState(ActorControlComponent::AIR);
}

}
