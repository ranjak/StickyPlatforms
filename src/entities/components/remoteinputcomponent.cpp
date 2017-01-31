#include "remoteinputcomponent.h"

void game::RemoteInputComponent::hitCommand(int key)
{
  hit((Command) key);
  hold((Command) key);
}

void game::RemoteInputComponent::releaseCommand(int key)
{
  release((Command) key);
}

void game::RemoteInputComponent::updateDelegate(std::uint32_t, Game &)
{
  // Keys hold their "hit" and "released" status for a single update cycle
  mHitCommands.clear();
  mReleasedCommands.clear();
}
