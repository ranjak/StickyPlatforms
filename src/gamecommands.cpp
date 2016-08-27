#include "gamecommands.h"
#include "SDL.h"

namespace game {


GameCommands::GameCommands() :
  bindings { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT }
{

}

void GameCommands::setBinding(Command cmdToBind, uint32_t scancode)
{
  bindings[cmdToBind] = scancode;
}

uint32_t GameCommands::getBinding(Command cmd)
{
  return bindings[cmd];
}

} // namespace game
