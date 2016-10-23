#include "climbingstate.h"
#include "actorcontrolcomponent.h"
#include "entity.h"
#include "movingphysicscomponent.h"
#include "inputcomponent.h"
#include "gamestate.h"
#include "collision.h"

namespace game {

ClimbingState::ClimbingState(ActorControlComponent &stateMachine) :
  ActorState(stateMachine),
  mClimbDirection(0),
  mInitialPosX(),
  mEdge()
{

}

void ClimbingState::enter()
{
  mStateMachine.physics().setGravityEnabled(false);

  const Rect<float> &box = mStateMachine.entity().getGlobalBox();

  mClimbDirection = mStateMachine.input().getDirection();
  mInitialPosX = box.x;

  const Level &level = GameState::current().getLevel();

  mEdge.y = level.getPixelSize().y;
  for (const Collision &col : mStateMachine.physics().getCollisions()) {

    if (col.isObstacle && col.normal.x == -mClimbDirection && col.bbox.y < mEdge.y)
      mEdge = col.bbox;
  }

  // Velocity boost to climb over the obstacle
  mStateMachine.physics().velocity().y = -400.f;
}

void ClimbingState::update(uint32_t step, GameState &game)
{
  const Rect<float> &pos = mStateMachine.entity().getGlobalBox();
  Vector<float> &velocity = mStateMachine.physics().velocity();

  if (pos.y + pos.h <= mEdge.y) {
    mStateMachine.physics().setGravityEnabled(true);
    velocity.y = 0.f;
    velocity.x = mClimbDirection * 100.f;
  }

  if (mInitialPosX != pos.x) {
    mStateMachine.setState(ActorControlComponent::AIR);
  }
}

void ClimbingState::exit()
{
  mStateMachine.physics().setGravityEnabled(true);

  mStateMachine.physics().velocity().y = 0.f;
  mStateMachine.physics().velocity().x = 0.f;
}

} // namespace game
