#include "airclingablestate.h"
#include "collision.h"
#include "actorcontrolcomponent.h"
#include "inputcomponent.h"
#include "gamecommands.h"

namespace game {

AirClingableState::AirClingableState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed, float friction) :
  AirState(stateMachine, maxSpeed, maxAirSpeed, friction)
{

}

void AirClingableState::receiveMessage(Message &msg)
{
  if (msg.type == Message::OnCollision) {
    Collision &colmsg = static_cast<Collision &>(msg);

    // For now, cling to tiles only.
    // TODO: add a "clingable" property, that can be used with entities as well
    if (colmsg.entity == Entity::none && colmsg.isObstacle && mStateMachine.input().isHeld(Command::JUMP) && colmsg.normal.y > 0) {
      mStateMachine.setState(ActorControlComponent::CLING);
      msg.accept();
    }
  }

  if (!msg.handled)
    AirState::receiveMessage(msg);
}

} // namespace game
