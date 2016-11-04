#include "groundstate.h"
#include "gamecommands.h"
#include "airstate.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "inputcomponent.h"
#include "make_unique.h"

namespace game {


GroundState::GroundState(ActorControlComponent &stateMachine, float maxSpeed) :
  HorizControlState(stateMachine, 3000.f, 5000.f, maxSpeed)
{

}

void GroundState::update(std::uint32_t step, Game &game)
{
  HorizControlState::update(step, game);

  if (mStateMachine.input().isHit(Command::JUMP))
    mStateMachine.setState(ActorControlComponent::JUMP);

  else if (!mStateMachine.physics().isOnGround())
    mStateMachine.setState(ActorControlComponent::AIR);
}

}
