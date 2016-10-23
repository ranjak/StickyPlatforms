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
}

AirState::AirState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed) :
    HorizControlState(stateMachine, 1200.f, maxSpeed),
    mMaxAirSpeed(maxAirSpeed)
{

}

void AirState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);
  MovingPhysicsComponent &physics = mStateMachine.physics();

  // Restrict movement in the air
  mMaxSpeed = std::max(std::abs(physics.velocity().x), mMaxAirSpeed);

  if (physics.isOnGround() && physics.velocity().y >= 0.f)
    mStateMachine.setState(ActorControlComponent::GROUND);
}

void AirState::receiveMessage(Message &msg)
{
  if (msg.type == Message::OnCollision) {

    Collision &col = static_cast<Collision &>(msg);

    // Special behavior if we're hugging a wall
    if (col.isObstacle && col.normal.x != 0) {

      //mStateMachine.setState(ActorControlComponent::WALLHUG);

      InputComponent &input = mStateMachine.input();
      // Walljump
      if (input.isHit(Command::JUMP)/* && input.getDirection() == col.normal.x*/) {
        glog(Log::DBG, "Walljump!");
        mStateMachine.physics().velocity().x = col.normal.x * mStateMachine.getMaxSpeed();
        mStateMachine.setState(ActorControlComponent::JUMP);
      }
      // Climb an edge
      else if (input.getDirection() == -col.normal.x) {

        const Level &level = GameState::current().getLevel();

        const Rect<float> &myBox = mStateMachine.entity().getGlobalBox();
        // The total space the climbing animation will take
        Rect<float> climbBox {
          (col.normal.x > 0) ? myBox.x-1.f : myBox.x,
          col.bbox.y - myBox.h,
          myBox.w + 1.f,
          myBox.h
        };


        // Climb if there aren't obstacles above
        if (distance(col.bbox.y, myBox.y) <= CLIMB_TOLERANCE && level.getObstaclesInArea(climbBox).empty()) {
          glog(Log::DBG, "Climbing! FloorY="<<col.bbox.y<<",ActorY="<<myBox.y);
          mStateMachine.setState(ActorControlComponent::CLIMB);
        }
      }
    }
  }
}

}
