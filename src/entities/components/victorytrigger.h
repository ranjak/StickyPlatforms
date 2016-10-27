#ifndef VICTORYTRIGGER_H
#define VICTORYTRIGGER_H

#include "component.h"
#include <string>

namespace game {

class Message;

/**
 * @brief A component that will trigger victory when the entity it belongs to
 * gets in contact with the hero (and the hero is on the ground).
 * It can be associated with a level to be loaded.
 */
class VictoryTrigger : public Component
{
public:
  VictoryTrigger(const std::string &nextLevel="");

  void receiveMessage(Message &msg) override;

private:
  std::string mNextLevel;
};

} // namespace game

#endif // VICTORYTRIGGER_H
