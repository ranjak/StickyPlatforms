#include "inputhandler.h"
#include <algorithm>

namespace game {

InputHandler::InputHandler() :
  event(),
  mQuitRequested(false),
  pressedKeys()
{

}


const std::vector<std::uint32_t>& InputHandler::handle()
{
  // Place the scancode of all currently pressed keys in pressedKeys

  while (SDL_PollEvent(&event)) {

    switch (event.type) {

    case SDL_QUIT:
      mQuitRequested = true;
      break;

    case SDL_KEYDOWN:
      // Only take this event into account if the user actually pressed the key
      if (!event.key.repeat)
        pressedKeys.push_back(event.key.keysym.scancode);
      break;

    case SDL_KEYUP:
      if (!event.key.repeat) {
        auto keyPos = std::find(pressedKeys.begin(), pressedKeys.end(), event.key.keysym.scancode);
        pressedKeys.erase(keyPos);
      }
      break;
    }
  }

  return pressedKeys;
}


bool InputHandler::quitRequested() const
{
  return mQuitRequested;
}


bool InputHandler::isKeyPressed(uint32_t scancode) const
{
  return std::find(pressedKeys.begin(), pressedKeys.end(), scancode) != pressedKeys.end();
}

}
