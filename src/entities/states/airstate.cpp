#include "airstate.h"
#include "gamevector.h"
#include "groundstate.h"
#include "gamestate.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "make_unique.h"
#include "collision.h"
#include "entity.h"
#include "inputcomponent.h"
#include "rect.h"
#include "log.h"
#include "util.h"
#include <algorithm>
#include <cmath>

namespace game {

namespace {
/** Zone in pixels above and below an edge where a climb animation can be triggered */
const float CLIMB_TOLERANCE = 5.f;
/** Time frame in milliseconds during which the user can perform a walljump */
const int WALLJUMP_TIME = 100;
}

AirState::AirState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed) :
    HorizControlState(stateMachine, 1200.f, maxSpeed),
    mMaxAirSpeed(maxAirSpeed),
    mWallHugNormal(0),
    mWalljumpTimeFrame(WALLJUMP_TIME)
{

}

void AirState::enter()
{
  mWallHugNormal = 0;
  mWalljumpTimeFrame = WALLJUMP_TIME;
}

void AirState::update(std::uint32_t step, GameState &game)
{
  MovingPhysicsComponent &physics = mStateMachine.physics();

  if (mWallHugNormal != 0) {
    updateWallHug(step, game);
  }
  else {

    HorizControlState::update(step, game);

    // Restrict movement in the air
    mMaxSpeed = std::max(std::abs(physics.velocity().x), mMaxAirSpeed);
  }

  if (physics.isOnGround() && physics.velocity().y >= 0.f)
    mStateMachine.setState(ActorControlComponent::GROUND);
}

void AirState::receiveMessage(Message &msg)
{
  if (msg.type == Message::OnCollision) {

    Collision &col = static_cast<Collision &>(msg);

    // Special behavior if we're hugging a wall
    if (col.isObstacle && col.normal.x != 0 && mStateMachine.input().getDirection() != col.normal.x) {
      mWallHugNormal = col.normal.x;
    }
  }
}

void AirState::updateWallHug(std::uint32_t step, GameState &game)
{
  MovingPhysicsComponent &physics = mStateMachine.physics();

  // Friction against the wall
  if (physics.velocity().y > 0) {
    physics.addAcceleration(Vector<float>(0.f, -MovingPhysicsComponent::GRAVITY/2.f));
  }

  // Make sure we're still in contact with the wall
  const std::vector<Collision> &collisions = mStateMachine.physics().getCollisions();
  Level &level = game.getLevel();

  bool wallContact = false;
  bool canClimb = true;

  for (const Collision &col : collisions) {

    if (col.isObstacle && col.normal.x == mWallHugNormal) {

      wallContact = true;
      canClimb = canClimb && col.normal.y <= 0;

      // If we're trying to climb, make sure we can
      if (canClimb && mStateMachine.input().getDirection() == -mWallHugNormal) {

        const Rect<float> &myBox = mStateMachine.entity().getGlobalBox();
        // The total space the climbing animation will take
        Rect<float> climbBox {
          (col.normal.x > 0) ? myBox.x-1.f : myBox.x,
          col.bbox.y - myBox.h,
          myBox.w + 1.f,
          myBox.h
        };

        // Climb if there aren't obstacles above
        // TODO check entities
        if (distance(col.bbox.y, myBox.y) <= CLIMB_TOLERANCE &&
            level.entities().getPhysics().getObstaclesInArea(climbBox).empty())
        {
          glog(Log::DBG, "Climbing! FloorY="<<col.bbox.y<<",ActorY="<<myBox.y);
          mStateMachine.setState(ActorControlComponent::CLIMB);
          return;
        }
      }
    }
    else if (col.isObstacle && col.normal.y > 0) {
      canClimb = false;
    }
  }

  // Check whether the player wants to do a walljump
  if (wallContact && mWalljumpTimeFrame >= 0 && mStateMachine.input().getDirection() == mWallHugNormal) {

    if (mStateMachine.input().isHit(Command::JUMP)) {
      glog(Log::DBG, "Walljump!");

      physics.velocity().x = mWallHugNormal * mStateMachine.getMaxSpeed();
      mStateMachine.setState(ActorControlComponent::JUMP);
    }

    mWalljumpTimeFrame -= step;
  }
  // If the player released the direction before the end of the timeframe without jumping, let go of the wall
  else if (!wallContact || mWalljumpTimeFrame < WALLJUMP_TIME) {
    glog(Log::DBG, "Lost wall contact");

    mWallHugNormal = 0;
    mWalljumpTimeFrame = WALLJUMP_TIME;
  }

}

}
