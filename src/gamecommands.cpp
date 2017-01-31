#include "gamecommands.h"
#include "SDL.h"

namespace game {

const int NbGameplayCommands = Command::RESET;

GameCommands::GameCommands(InputHandler &input) :
  mBindings { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_SPACE, SDL_SCANCODE_F,
    SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_ESCAPE, {SDL_SCANCODE_RETURN, ModifierKey::ALT} },
  mInput(input)
{

}

void GameCommands::setBinding(Command cmdToBind, const KeyBinding &binding)
{
  mBindings[cmdToBind] = binding;
}

const KeyBinding &GameCommands::getBinding(Command cmd) const
{
  return mBindings[cmd];
}

bool GameCommands::isHit(Command cmd) const
{
  if (mBindings[cmd].modifier != ModifierKey::NONE && (!mInput.isModifierPressed(mBindings[cmd].modifier)))
    return false;

  return mInput.isKeyHit(mBindings[cmd].keyScancode);
}

bool GameCommands::isHeld(Command cmd) const
{
  if (mBindings[cmd].modifier != ModifierKey::NONE && (!mInput.isModifierPressed(mBindings[cmd].modifier)))
    return false;

  return mInput.isKeyHeld(mBindings[cmd].keyScancode);
}

bool GameCommands::isReleased(Command cmd) const
{
  return mInput.isKeyReleased(mBindings[cmd].keyScancode);
}

} // namespace game
