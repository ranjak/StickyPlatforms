#include "inputsendercomponent.h"
#include "gamecommands.h"
#include "inputcomponent.h"
#include "game.h"
#include "stormancerconnection.h"
#include <vector>
#include <algorithm>

game::InputSenderComponent::InputSenderComponent(const InputComponent & input) :
  //mPressedKeys{ false },
  mInput(input)
{
}

void game::InputSenderComponent::updateDelegate(std::uint32_t step, Game & game)
{
  std::vector<int> hitKeys;
  std::vector<int> releasedKeys;

  for (int key = 0; key < NbGameplayCommands; key++) {

    if (mInput.isHit((Command) key))
      hitKeys.push_back(key);

    else if (mInput.isReleased((Command) key))
      releasedKeys.push_back(key);
  }

  if (!hitKeys.empty() || !releasedKeys.empty())
    game.network().updateKeys(hitKeys, releasedKeys);
}
