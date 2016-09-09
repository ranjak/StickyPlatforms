#include "gamestate.h"
#include "hero.h"
#include "platform.h"
#include "test/test.h"
#include <algorithm>

namespace game {

const std::uint32_t TIMESTEP = 10;

GameState::GameState() :
  mBindings(),
  mInputSnapshot(),
  mLevel(std::move(Test::makeLevel()))
{
}


void GameState::update(uint32_t step, const InputHandler *input)
{
  mInputSnapshot = input;

  mLevel->update(*this, step);
}

void GameState::draw(Display &target) const
{
  mLevel->draw(target);
}

bool GameState::isCommandPressed(Command cmd)
{
  return mInputSnapshot->isKeyPressed(mBindings.getBinding(cmd));
}

Level &GameState::getLevel()
{
  return *mLevel;
}

} //namespace game
