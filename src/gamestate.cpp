#include "gamestate.h"
#include "hero.h"
#include "display.h"
#include "test/test.h"
#include "world/level.h"
#include <algorithm>

namespace game {

const std::uint32_t TIMESTEP = 10;

GameState *GameState::currentGame = nullptr;

const GameState &GameState::current()
{
  return *currentGame;
}

GameState::GameState(Display &display, InputHandler &input, int camW, int camH) :
  mCommands(input),
  mLevel(std::move(Test::makeLevel(display))),
  mCamera(0, 0, camW, camH),
  mGameTime(0)
{
  display.setCameraSize(camW, camH);
  currentGame = this;
}


void GameState::update(uint32_t step)
{
  mGameTime += step;
  mLevel->update(*this, step);
}

void GameState::draw(Display &target) const
{
  mLevel->draw(target, *this);
}

GameCommands &GameState::getCommands()
{
  return mCommands;
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

uint32_t GameState::now() const
{
  return mGameTime;
}

} //namespace game
