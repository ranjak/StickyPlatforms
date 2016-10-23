#include "basicaicomponent.h"
#include "collision.h"
#include "make_unique.h"
#include "gamecommands.h"
#include "gamevector.h"
#include "movingphysicscomponent.h"
#include "rect.h"

namespace game {

BasicAiComponent::BasicAiComponent(MovingPhysicsComponent &physics) :
  AutoInputComponent(),
  mMsgQueue(),
  mPhysics(physics)
{
  hold(Command::LEFT);
}

void BasicAiComponent::receiveMessage(Message &message)
{
  switch (message.type) {
  default:
    break;
  }
}

void BasicAiComponent::processMessages()
{
  while (!mMsgQueue.empty()) {

    Message *msg = mMsgQueue.front().get();

    switch (msg->type) {
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

  // Turn around when we reach the edge of a platform or a wall

  const Rect<float> &ebox = mPhysics.entity().getGlobalBox();
  bool turnAround = true;

  if (isHeld(Command::RIGHT)) {
    for (const Collision &col : mPhysics.getCollisions()) {

      // Facing a wall ?
      if (col.normal.x < 0) {
        turnAround = true;
        break;
      }
      // Over a cliff ?
      else if (col.normal.y < 0 && (col.bbox.x+col.bbox.w >= ebox.x+ebox.w))
        turnAround = false;
    }
    if (turnAround) {
      release(Command::RIGHT);
      hold(Command::LEFT);
    }
  }

  else if (isHeld(Command::LEFT)) {
    for (const Collision &col : mPhysics.getCollisions()) {

      if (col.normal.x > 0) {
        turnAround = true;
        break;
      }
      else if (col.normal.y < 0 && (col.bbox.x <= ebox.x))
        turnAround = false;
    }
    if (turnAround) {
      release(Command::LEFT);
      hold(Command::RIGHT);
    }
  }
}

} // namespace game
