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
#include <algorithm>
#include <cmath>

namespace game {

AirState::AirState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed) :
    HorizControlState(stateMachine, 1200.f, maxSpeed),
    mMaxAirSpeed(maxAirSpeed)
{

}

void AirState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  // Restrict movement in the air
  mMaxSpeed = std::max(std::abs(mStateMachine.physics().velocity().x), mMaxAirSpeed);

  if (mStateMachine.physics().isOnGround())
    mStateMachine.setState(ActorControlComponent::GROUND);
}

void AirState::receiveMessage(Message &msg)
{
  if (msg.type == Message::Collision) {

    CollisionMsg &col = static_cast<CollisionMsg &>(msg);

    // Special behavior if we're hugging a wall
    if (col.entity == Entity::none && col.normal.x != 0) {

      InputComponent &input = mStateMachine.input();
      // Walljump
      if (input.isHit(Command::JUMP) && input.getDirection() == col.normal.x) {
        glog(Log::DBG, "Walljump!");
        mStateMachine.physics().velocity().x = col.normal.x * 300.f;
        mStateMachine.physics().velocity().y = -800.f;
      }
      // Climb an edge
      else if (input.isHeld(Command::JUMP) && input.getDirection() == -col.normal.x) {

        const Level &level = GameState::current().getLevel();

        // Make sure we aren't too low below floor level
        const Rect<float> &tileBox = level.getTileAt(col.tilePos)->getCollisionBox(col.tilePos.x, col.tilePos.y);
        const Rect<float> &myBox = mStateMachine.entity().getGlobalBox();

        // Also make sure there aren't obstacles above
        const Tile *aboveFloor = level.getTileAt(Vector<int>(col.tilePos.x, col.tilePos.y-1));
        const Tile *aboveActor = level.getTileAt(Vector<int>(myBox.x / Tile::SIZE, col.tilePos.y-1));

        if (myBox.y <= tileBox.y && (!aboveActor || !aboveActor->isObstacle()) && (!aboveFloor || !aboveFloor->isObstacle())) {
          glog(Log::DBG, "Climbing!");
          mStateMachine.setState(ActorControlComponent::CLIMB);
        }
      }
    }
  }
}

}
