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
   * @return List of keys currently being pressed.
   */
  const std::vector<std::uint32_t>& handle();

  bool quitRequested() const;
  bool isKeyPressed(uint32_t scancode) const;
private:
  SDL_Event event;
  bool mQuitRequested;

  // TODO optimize/simplify (use SDL's input state table ?)
  std::vector<std::uint32_t> pressedKeys;
};

}

#endif // INPUTHANDLER_H
