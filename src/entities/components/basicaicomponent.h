#ifndef BASICAICOMPONENT_H
#define BASICAICOMPONENT_H

#include "autoinputcomponent.h"
#include "message.h"
#include <queue>
#include <list>
#include <memory>

namespace game {


class BasicAiComponent : public AutoInputComponent
{
public:
  BasicAiComponent();

  void update(uint32_t step, GameState &game) override;

  /**
   * @brief receiveMessage Copies relevant messages onto a local queue for processing at the next update.
   * This isolates AI logic in the update phase.
   * @param message
   */
  void receiveMessage(Message &message) override;

private:
  void processMessages();

private:
  int mDirection;
  std::queue<std::unique_ptr<Message>, std::list<std::unique_ptr<Message>>> mMsgQueue;
};

} // namespace game

#endif // BASICAICOMPONENT_H
