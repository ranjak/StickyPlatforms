#include "sdlinputhandler.h"
#include "mainloop.h"
#include "log.h"
#include <algorithm>

namespace game {

// ModifierKey to SDL_Keymod lookup table
static std::uint16_t sdlKeymod[] = {
  KMOD_NONE,
  KMOD_ALT
};

SDLInputHandler::SDLInputHandler() :
  InputHandler(),
  mEvent(),
  mMinimized(false)
{

}

void SDLInputHandler::handle()
{
  // Hit and Release events last only one frame
  mHitKeys.clear();
  mReleasedKeys.clear();

  // Pause the whole program when minimized
  while (mMinimized) {

    if (!SDL_WaitEvent(&mEvent))
      game::error(std::string("SDLInputHandler: Event error: ") + SDL_GetError());

    else if (mEvent.type == SDL_WINDOWEVENT &&
             (mEvent.window.event == SDL_WINDOWEVENT_SHOWN || mEvent.window.event == SDL_WINDOWEVENT_EXPOSED))
    {
      mMinimized = false;
      MainLoop::setBlocked(false);
    }
  }

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

    case SDL_WINDOWEVENT:
      if ((mEvent.window.event == SDL_WINDOWEVENT_HIDDEN || mEvent.window.event == SDL_WINDOWEVENT_MINIMIZED) &&
          !(SDL_GetWindowFlags(SDL_GetWindowFromID(mEvent.window.windowID)) & SDL_WINDOW_SHOWN))
      {
        mMinimized = true;
        MainLoop::setBlocked(true);
      }
      break;
    }
  }
}

bool SDLInputHandler::isModifierPressed(ModifierKey modifier) const
{
  return SDL_GetModState() & sdlKeymod[static_cast<int>(modifier)];
}

bool SDLInputHandler::applicationMinimized() const
{
  return mMinimized;
}

} // namespace game
