#include "airstate.h"
#include "gamevector.h"
#include "groundstate.h"
#include "gamestate.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "make_unique.h"

namespace game {

AirState::AirState(ActorControlComponent &stateMachine, float maxSpeed) :
    HorizControlState(stateMachine, 1200.f, maxSpeed)
{

}

void AirState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  if (mStateMachine.physics().isOnGround())
    mStateMachine.setState(ActorControlComponent::GROUND);
}

}
