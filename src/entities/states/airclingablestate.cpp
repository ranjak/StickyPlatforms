#include "airclingablestate.h"
#include "collision.h"
#include "actorcontrolcomponent.h"
#include "inputcomponent.h"
#include "gamecommands.h"

namespace game {

AirClingableState::AirClingableState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed) :
  AirState(stateMachine, maxSpeed, maxAirSpeed)
{

}

void AirClingableState::receiveMessage(Message &msg)
{
  AirState::receiveMessage(msg);

  switch (msg.type) {
  case Message::OnCollision:
  {
    Collision &colmsg = static_cast<Collision &>(msg);

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
