#include "basicaicomponent.h"
#include "collisionmsg.h"
#include "make_unique.h"
#include "gamecommands.h"
#include "gamevector.h"

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

  case Message::Collision:
    mMsgQueue.push(std::make_unique<CollisionMsg>(static_cast<CollisionMsg &>(message)));
  default:
    break;
  }
}

void BasicAiComponent::processMessages()
{
  while (!mMsgQueue.empty()) {

    Message *msg = mMsgQueue.front().get();

    switch (msg->type) {
    case Message::Collision:
    {
      CollisionMsg *colmsg = static_cast<CollisionMsg *>(msg);

      if (colmsg->isObstacle)
      {
        if (isHeld(Command::RIGHT) && colmsg->normal.x < 0) {
          release(Command::RIGHT);
          hold(Command::LEFT);
        }
        else if (isHeld(Command::LEFT) && colmsg->normal.x > 0) {
          release(Command::LEFT);
          hold(Command::RIGHT);
        }
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
