#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "component.h"
#include "gamecommands.h"

namespace game {

/**
 * @brief The InputComponent class is the interface for game entities that receive input.
 */
class InputComponent : public Component
{
public:
  virtual bool isHit(Command cmd) const = 0;
  virtual bool isHeld(Command cmd) const = 0;
  virtual bool isReleased(Command cmd) const = 0;
};

} // namespace game

#endif // INPUTCOMPONENT_H
