#include "gamestate.h"
#include "hero.h"
#include "platform.h"
#include "test/test.h"
#include <algorithm>

namespace game {

const std::uint32_t TIMESTEP = 10;

GameState::GameState(Display &display) :
  mBindings(),
  mInputSnapshot(),
  mLevel(std::move(Test::makeLevel(display))),
  mCamera(0, 0, 320, 240)
{
}


void GameState::update(uint32_t step, const InputHandler *input)
{
  mInputSnapshot = input;

  mLevel->update(*this, step);
}

void GameState::draw(Display &target) const
{
  mLevel->draw(target, *this);
}

bool GameState::isCommandPressed(Command cmd)
{
  return mInputSnapshot->isKeyPressed(mBindings.getBinding(cmd));
}

Level &GameState::getLevel()
{
  return *mLevel;
}

Camera &GameState::getCamera()
{
  return mCamera;
}

const Camera &GameState::getCamera() const
{
  return mCamera;
}

} //namespace game
