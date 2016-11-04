#ifndef BASICAICOMPONENT_H
#define BASICAICOMPONENT_H

#include "autoinputcomponent.h"
#include "message.h"
#include <queue>
#include <list>
#include <memory>

namespace game {

class MovingPhysicsComponent;

class BasicAiComponent : public AutoInputComponent
{
public:
  BasicAiComponent(MovingPhysicsComponent &physics);


private:
  void processMessages();

  void updateDelegate(uint32_t step, Game &game) override;

  /**
   * @brief receiveMessage Copies relevant messages onto a local queue for processing at the next update.
   * This isolates AI logic in the update phase.
   * @param message
   */
  void receiveMessageDelegate(Message &message) override;

private:
  int mDirection;
  std::queue<std::unique_ptr<Message>, std::list<std::unique_ptr<Message>>> mMsgQueue;
  MovingPhysicsComponent &mPhysics;
};

} // namespace game

#endif // BASICAICOMPONENT_H
