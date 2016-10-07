#include "basicaicomponent.h"
#include "obstaclereachedmsg.h"
#include "directionchangedmsg.h"
#include "entity.h"
#include "make_unique.h"
#include "gamecommands.h"

namespace game {

BasicAiComponent::BasicAiComponent() :
  AutoInputComponent(),
  mMsgQueue()
{
  hold(Command::LEFT);
}

void BasicAiComponent::receiveMessage(Message &message)
{
  switch (message.type) {

  case Message::ObstacleReached:
    mMsgQueue.push(std::make_unique<ObstacleReachedMsg>(static_cast<ObstacleReachedMsg &>(message)));
  default:
    break;
  }
}

void BasicAiComponent::processMessages()
{
  while (!mMsgQueue.empty()) {

    Message *msg = mMsgQueue.front().get();

    switch (msg->type) {
    case Message::ObstacleReached:
    {
      int normalX = static_cast<ObstacleReachedMsg *>(msg)->normal.x;

      if (isHeld(Command::RIGHT) && normalX < 0) {
        release(Command::RIGHT);
        hold(Command::LEFT);
      }
      else if (isHeld(Command::LEFT) && normalX > 0) {
        release(Command::LEFT);
        hold(Command::RIGHT);
      }

      break;
    }
    default:
      break;
    }

    mMsgQueue.pop();
  }
}

void BasicAiComponent::update(uint32_t step, GameState &game)
{
  AutoInputComponent::update(step, game);

  processMessages();
}

} // namespace game
