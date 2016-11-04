#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H

#include "inputcomponent.h"

namespace game {

class Game;

/**
 * @brief The PlayerInputComponent class simply passes user input through.
 */
class PlayerInputComponent : public InputComponent
{
public:

  bool isHit(Command cmd) const override;
  bool isHeld(Command cmd) const override;
  bool isReleased(Command cmd) const override;
};

} // namespace game

#endif // PLAYERINPUTCOMPONENT_H
