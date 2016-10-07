#include "playerinputcomponent.h"
#include "gamestate.h"

namespace game {

bool PlayerInputComponent::isHit(Command cmd) const
{
  return GameState::current().getCommands().isHit(cmd);
}

bool PlayerInputComponent::isHeld(Command cmd) const
{
  return GameState::current().getCommands().isHeld(cmd);
}

bool PlayerInputComponent::isReleased(Command cmd) const
{
  return GameState::current().getCommands().isReleased(cmd);
}

} // namespace game
