#include "gamecommands.h"
#include "inputhandler.h"
#include "SDL.h"

namespace game {


GameCommands::GameCommands(InputHandler &input) :
  mBindings { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_SPACE, SDL_SCANCODE_F },
  mInput(input)
{

}

void GameCommands::setBinding(Command cmdToBind, std::uint32_t scancode)
{
  mBindings[cmdToBind] = scancode;
}

std::uint32_t GameCommands::getBinding(Command cmd) const
{
  return mBindings[cmd];
}

bool GameCommands::isHit(Command cmd) const
{
  return mInput.isKeyHit(mBindings[cmd]);
}

bool GameCommands::isHeld(Command cmd) const
{
  return mInput.isKeyHeld(mBindings[cmd]);
}

bool GameCommands::isReleased(Command cmd) const
{
  return mInput.isKeyReleased(mBindings[cmd]);
}

} // namespace game
