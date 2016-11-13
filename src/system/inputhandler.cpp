#include "inputhandler.h"
#include <algorithm>

namespace game {

InputHandler::InputHandler() :
  mHitKeys(),
  mHeldKeys(),
  mReleasedKeys()
{

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
