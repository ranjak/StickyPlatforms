#include "inputhandler.h"
#include <algorithm>

namespace game {

InputHandler::InputHandler() :
  mEvent(),
  mQuitRequested(false),
  mHeldKeys()
{

}


void InputHandler::handle()
{
  // Hit and Release events last only one frame
  mHitKeys.clear();
  mReleasedKeys.clear();

  while (SDL_PollEvent(&mEvent)) {

    switch (mEvent.type) {

    case SDL_QUIT:
      mQuitRequested = true;
      break;

    case SDL_KEYDOWN:
      // Only take this event into account if the user actually pressed the key
      if (!mEvent.key.repeat) {
        mHitKeys.push_back(mEvent.key.keysym.scancode);
        mHeldKeys.push_back(mEvent.key.keysym.scancode);
      }
      break;

    case SDL_KEYUP:
      if (!mEvent.key.repeat) {
        mReleasedKeys.push_back(mEvent.key.keysym.scancode);
        auto keyPos = std::find(mHeldKeys.begin(), mHeldKeys.end(), mEvent.key.keysym.scancode);
        mHeldKeys.erase(keyPos);
      }
      break;
    }
  }
}


bool InputHandler::quitRequested() const
{
  return mQuitRequested;
}


bool InputHandler::isKeyHit(uint32_t scancode) const
{
  return std::find(mHitKeys.begin(), mHitKeys.end(), scancode) != mHitKeys.end();
}

bool InputHandler::isKeyHeld(uint32_t scancode) const
{
  return std::find(mHeldKeys.begin(), mHeldKeys.end(), scancode) != mHeldKeys.end();
}

bool InputHandler::isKeyReleased(uint32_t scancode) const
{
  return std::find(mReleasedKeys.begin(), mReleasedKeys.end(), scancode) != mReleasedKeys.end();
}

}
