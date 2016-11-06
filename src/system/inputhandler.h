#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "SDL.h"
#include <vector>
#include <cstdint>

namespace game {

class InputHandler
{
public:
  InputHandler();

  /**
   * @brief Handle player input for one frame.
   */
  void handle();

  bool isKeyHit(std::uint32_t scancode) const;
  bool isKeyHeld(std::uint32_t scancode) const;
  bool isKeyReleased(std::uint32_t scancode) const;

private:
  SDL_Event mEvent;

  // Keys that were just pressed at the time handle() was called
  std::vector<std::uint32_t> mHitKeys;
  // Keys that were in mHitKeys last time handle() was called and haven't been released
  std::vector<std::uint32_t> mHeldKeys;
  // Keys that were just released
  std::vector<std::uint32_t> mReleasedKeys;
};

}

#endif // INPUTHANDLER_H
