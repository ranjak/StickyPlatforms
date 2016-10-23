#include "wallhugstate.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "gamevector.h"
#include "gamestate.h"
#include "inputcomponent.h"
#include "world/level.h"
#include "physicsmanager.h"
#include "collision.h"
#include "util.h"
#include "log.h"

namespace game {

/** Zone in pixels above and below an edge where a climb animation can be triggered */
static const float CLIMB_TOLERANCE = 5.f;

/** Time frame in milliseconds during which the user can perform a walljump */
static const std::uint32_t WALLJUMP_TIME = 100;

WallHugState::WallHugState(ActorControlComponent &stateMachine) :
  ActorState(stateMachine),
  mWalljumpTimeFrame(WALLJUMP_TIME)
{

}

void WallHugState::enter()
{
  mWalljumpTimeFrame = WALLJUMP_TIME;
}

void WallHugState::update(uint32_t step, GameState &game)
{
  MovingPhysicsComponent &physics = mStateMachine.physics();

  if (physics.isOnGround()) {
    mStateMachine.setState(ActorControlComponent::GROUND);
    return;
  }

  // Friction against the wall
  if (physics.velocity().y > 0) {
    physics.addAcceleration(Vector<float>(0.f, -MovingPhysicsComponent::GRAVITY/2.f));
  }

  // Make sure we're still in contact with the wall
  const std::vector<Collision> &collisions = mStateMachine.physics().getCollisions();
  Level &level = game.getLevel();

  int wallNormal = 0;
  bool canClimb = true;

  for (const Collision &col : collisions) {

    if (col.isObstacle) {

      canClimb = canClimb && col.normal.y <= 0;
      if (col.normal.x != 0)
        wallNormal = col.normal.x;

      // If we're trying to climb, make sure we can
      if (canClimb && col.normal.x != 0 && mStateMachine.input().getDirection() == -col.normal.x) {

        const Rect<float> &myBox = mStateMachine.entity().getGlobalBox();

        // Make sure we aren't too low below floor level
        // Also check the climb animation won't be blocked
        // TODO check entities
        if (distance(col.bbox.y, myBox.y) <= CLIMB_TOLERANCE &&
            level.entities().getPhysics().getObstaclesInArea(Rect<float>(myBox.x, col.bbox.y-myBox.h, myBox.w+1.f, myBox.h)).empty())
        {
          glog(Log::DBG, "Climbing! FloorY="<<col.bbox.y<<",ActorY="<<myBox.y);
          mStateMachine.setState(ActorControlComponent::CLIMB);
          return;
        }
      }
    }
  }

  // Check whether the player wants to do a walljump
  if (wallNormal != 0 && mWalljumpTimeFrame > 0 && mStateMachine.input().getDirection() == wallNormal) {

    if (mStateMachine.input().isHit(Command::JUMP)) {
      glog(Log::DBG, "Walljump!");

      physics.velocity().x = wallNormal * mStateMachine.getMaxSpeed();
      mStateMachine.setState(ActorControlComponent::JUMP);
    }

    mWalljumpTimeFrame -= step;
  }
  // If the player released the direction before the end of the timeframe without jumping, let go of the wall
  else if (wallNormal == 0 || mWalljumpTimeFrame < WALLJUMP_TIME) {
    glog(Log::DBG, "Lost wall contact");

    mStateMachine.setState(ActorControlComponent::AIR_CLINGABLE);
  }
}

} // namespace game
