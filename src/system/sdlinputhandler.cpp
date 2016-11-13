#include "sdlinputhandler.h"
#include "mainloop.h"
#include <algorithm>

namespace game {

// ModifierKey to SDL_Keymod lookup table
static std::uint16_t sdlKeymod[] = {
  KMOD_NONE,
  KMOD_ALT
};

SDLInputHandler::SDLInputHandler() :
  InputHandler(),
  mEvent()
{

}

void SDLInputHandler::handle()
{
  // Hit and Release events last only one frame
  mHitKeys.clear();
  mReleasedKeys.clear();

  while (SDL_PollEvent(&mEvent)) {

    switch (mEvent.type) {

    case SDL_QUIT:
      MainLoop::requestExit();
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

bool SDLInputHandler::isModifierPressed(ModifierKey modifier) const
{
  return SDL_GetModState() & sdlKeymod[static_cast<int>(modifier)];
}

} // namespace game
