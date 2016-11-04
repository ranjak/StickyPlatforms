#include "playerinputcomponent.h"
#include "game.h"

namespace game {

bool PlayerInputComponent::isHit(Command cmd) const
{
  return Game::current().getCommands().isHit(cmd);
}

bool PlayerInputComponent::isHeld(Command cmd) const
{
  return Game::current().getCommands().isHeld(cmd);
}

bool PlayerInputComponent::isReleased(Command cmd) const
{
  return Game::current().getCommands().isReleased(cmd);
}

} // namespace game
