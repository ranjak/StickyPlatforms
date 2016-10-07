#include "autoinputcomponent.h"
#include "gamecommands.h"
#include <algorithm>
#include <cassert>

namespace game {


void AutoInputComponent::update(uint32_t /*step*/, GameState &/*game*/)
{
  mReleasedCommands.clear();
  mReleasedCommands.swap(mHitCommands);
}

bool AutoInputComponent::isHit(Command cmd) const
{
  return std::find(mHitCommands.begin(), mHitCommands.end(), cmd) != mHitCommands.end();
}

bool AutoInputComponent::isHeld(Command cmd) const
{
  return std::find(mHeldCommands.begin(), mHeldCommands.end(), cmd) != mHeldCommands.end();
}

bool AutoInputComponent::isReleased(Command cmd) const
{
  return std::find(mReleasedCommands.begin(), mReleasedCommands.end(), cmd) != mReleasedCommands.end();
}

void AutoInputComponent::hit(Command cmd)
{
  mHitCommands.push_back(cmd);
}

void AutoInputComponent::hold(Command cmd)
{
  mHeldCommands.push_back(cmd);
}

void AutoInputComponent::release(Command cmd)
{
  assert(std::find(mHeldCommands.begin(), mHeldCommands.end(), cmd) != mHeldCommands.end());

  mHeldCommands.erase(std::remove(mHeldCommands.begin(), mHeldCommands.end(), cmd));
  mReleasedCommands.push_back(cmd);
}

} // namespace game
