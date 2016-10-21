#include "airstate.h"
#include "gamevector.h"
#include "groundstate.h"
#include "gamestate.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "make_unique.h"
#include "collisionmsg.h"
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
  if (msg.type == Message::Collision) {

    CollisionMsg &col = static_cast<CollisionMsg &>(msg);

    // Special behavior if we're hugging a wall
    if (col.entity == Entity::none && col.normal.x != 0) {

      mStateMachine.setState(ActorControlComponent::WALLHUG);

      /*
      InputComponent &input = mStateMachine.input();
      // Walljump
      if (input.isHit(Command::JUMP) && input.getDirection() == col.normal.x) {
        glog(Log::DBG, "Walljump!");
        mStateMachine.physics().velocity().x = col.normal.x * mStateMachine.getMaxSpeed();
        mStateMachine.setState(ActorControlComponent::JUMP);
      }
      // Climb an edge
      else if (input.getDirection() == -col.normal.x) {

        const Level &level = GameState::current().getLevel();

        // Make sure we aren't too low below floor level
        const Rect<float> &tileBox = level.getTileAt(col.tilePos)->getCollisionBox(col.tilePos.x, col.tilePos.y);
        const Rect<float> &myBox = mStateMachine.entity().getGlobalBox();

        // Also make sure there aren't obstacles above
        const Tile *aboveFloor = level.getTileAt(Vector<int>(col.tilePos.x, col.tilePos.y-1));
        const Tile *aboveActor = level.getTileAt(Vector<int>(myBox.x / Tile::SIZE, col.tilePos.y-1));

        if (distance(tileBox.y, myBox.y) <= CLIMB_TOLERANCE && (!aboveActor || !aboveActor->isObstacle()) && (!aboveFloor || !aboveFloor->isObstacle())) {
          glog(Log::DBG, "Climbing! FloorY="<<tileBox.y<<",ActorY="<<myBox.y);
          mStateMachine.setState(ActorControlComponent::CLIMB);
        }
      }
      */
    }
  }
}

}
