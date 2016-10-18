#include "airclingablestate.h"
#include "collisionmsg.h"
#include "actorcontrolcomponent.h"
#include "inputcomponent.h"
#include "gamecommands.h"

namespace game {

AirClingableState::AirClingableState(ActorControlComponent &stateMachine, float maxSpeed) :
  AirState(stateMachine, maxSpeed)
{

}

void AirClingableState::receiveMessage(Message &msg)
{
  switch (msg.type) {
  case Message::Collision:
  {
    CollisionMsg &colmsg = static_cast<CollisionMsg &>(msg);

    // For now, cling to tiles only.
    // TODO: add a "clingable" property, that can be used with entities as well
    if (colmsg.entity == Entity::none && colmsg.isObstacle && mStateMachine.input().isHeld(Command::JUMP) && colmsg.normal.y > 0) {
      mStateMachine.setState(ActorControlComponent::CLING);
    }
    break;
  }
  default:
    break;
  }
}

} // namespace game
