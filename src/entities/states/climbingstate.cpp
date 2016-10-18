#include "climbingstate.h"
#include "actorcontrolcomponent.h"
#include "gravitycomponent.h"
#include "entity.h"
#include "movingphysicscomponent.h"
#include "inputcomponent.h"

namespace game {

ClimbingState::ClimbingState(ActorControlComponent &stateMachine) :
  ActorState(stateMachine),
  mClimbDirection(0)
{

}

void ClimbingState::enter()
{
  GravityComponent *gravity = mStateMachine.entity().getComponent<GravityComponent>();
  if (gravity)
    gravity->setEnabled(false);

  mClimbDirection = mStateMachine.input().getDirection();
  mInitialPosX = mStateMachine.entity().getGlobalBox().x;

  // Velocity boost to climb over the obstacle
  mStateMachine.physics().velocity().y = -400.f;
}

void ClimbingState::update(uint32_t step, GameState &game)
{
  if (mInitialPosX != mStateMachine.entity().getGlobalBox().x) {
    mStateMachine.setState(ActorControlComponent::AIR);
  }

  mStateMachine.physics().velocity().x = mClimbDirection * 100.f;
}

void ClimbingState::exit()
{
  GravityComponent *gravity = mStateMachine.entity().getComponent<GravityComponent>();
  if (gravity)
    gravity->setEnabled(true);

  mStateMachine.physics().velocity().y = 0.f;
}

} // namespace game
