#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "SDL.h"

namespace game {

class InputHandler
{
public:
  InputHandler();
  /**
     * @brief Handle player input for one frame.
     */
  void handle();

  bool quitRequested();
private:
  SDL_Event event;
  bool mQuitRequested;
};

}

#endif // INPUTHANDLER_H
