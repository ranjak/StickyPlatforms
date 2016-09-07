#include "gamecommands.h"
#include "SDL.h"

namespace game {


GameCommands::GameCommands() :
  mBindings { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_SPACE }
{

}

void GameCommands::setBinding(Command cmdToBind, uint32_t scancode)
{
  mBindings[cmdToBind] = scancode;
}

uint32_t GameCommands::getBinding(Command cmd)
{
  return mBindings[cmd];
}

} // namespace game
