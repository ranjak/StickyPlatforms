#include "inputhandler.h"

namespace game {

InputHandler::InputHandler() :
  event(),
  mQuitRequested(false)
{

}

void InputHandler::handle()
{
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    mQuitRequested = true;
    break;
  }
}

bool InputHandler::quitRequested()
{
  return mQuitRequested;
}

}
