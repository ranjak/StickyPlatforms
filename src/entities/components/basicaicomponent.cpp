#include "basicaicomponent.h"
#include "collision.h"
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

  case Message::OnCollision:
    mMsgQueue.push(std::make_unique<Collision>(static_cast<Collision &>(message)));
  default:
    break;
  }
}

void BasicAiComponent::processMessages()
{
  while (!mMsgQueue.empty()) {

    Message *msg = mMsgQueue.front().get();

    switch (msg->type) {
    case Message::OnCollision:
    {
      Collision *colmsg = static_cast<Collision *>(msg);

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
