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

  bool quitRequested() const;
  bool isKeyPressed(uint32_t scancode) const;
private:
  SDL_Event event;
  bool mQuitRequested;

  std::vector<std::uint32_t> pressedKeys;
};

}

#endif // INPUTHANDLER_H
