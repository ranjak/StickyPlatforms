#include "basicaicomponent.h"
#include "obstaclereachedmsg.h"
#include "directionchangedmsg.h"
#include "entity.h"

namespace game {

BasicAiComponent::BasicAiComponent(Entity &owner) :
  mOwner(owner)
{

}

void BasicAiComponent::receiveMessage(Message &message)
{
  switch (message.type) {

  case Message::ObstacleReached:
  {
    int normalX = static_cast<ObstacleReachedMsg &>(message).normal.x;

    if (normalX != 0)
      mOwner.sendMessage(std::unique_ptr<Message>(new DirectionChangedMsg(normalX)));

    break;
  }
  default:
    break;
  }
}

} // namespace game
