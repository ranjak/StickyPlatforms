#ifndef SDLINPUTHANDLER_H
#define SDLINPUTHANDLER_H

#include "inputhandler.h"
#include "SDL.h"
#include <cstdint>

namespace game {

class SDLInputHandler : public InputHandler
{
public:
  SDLInputHandler();

  /**
   * @brief Handle player input for one frame.
   */
  void handle() override;

  bool isModifierPressed(ModifierKey modifier) const override;

private:
  SDL_Event mEvent;
};

} // namespace game

#endif // SDLINPUTHANDLER_H
