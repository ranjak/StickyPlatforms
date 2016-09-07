#include "inputhandler.h"
#include <algorithm>

namespace game {

InputHandler::InputHandler() :
  mEvent(),
  mQuitRequested(false),
  mPressedKeys()
{

}


const std::vector<std::uint32_t>& InputHandler::handle()
{
  // Place the scancode of all currently pressed keys in pressedKeys

  while (SDL_PollEvent(&mEvent)) {

    switch (mEvent.type) {

    case SDL_QUIT:
      mQuitRequested = true;
      break;

    case SDL_KEYDOWN:
      // Only take this event into account if the user actually pressed the key
      if (!mEvent.key.repeat)
        mPressedKeys.push_back(mEvent.key.keysym.scancode);
      break;

    case SDL_KEYUP:
      if (!mEvent.key.repeat) {
        auto keyPos = std::find(mPressedKeys.begin(), mPressedKeys.end(), mEvent.key.keysym.scancode);
        mPressedKeys.erase(keyPos);
      }
      break;
    }
  }

  return mPressedKeys;
}


bool InputHandler::quitRequested() const
{
  return mQuitRequested;
}


bool InputHandler::isKeyPressed(uint32_t scancode) const
{
  return std::find(mPressedKeys.begin(), mPressedKeys.end(), scancode) != mPressedKeys.end();
}

}
