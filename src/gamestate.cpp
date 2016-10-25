#include "gamestate.h"
#include "display.h"
#include "test/test.h"
#include "world/level.h"
#include "log.h"
#include <algorithm>
#include <stdexcept>

namespace game {

const std::uint32_t TIMESTEP = 10;

GameState *GameState::currentGame = nullptr;

const GameState &GameState::current()
{
  return *currentGame;
}

GameState::GameState(Display &display, InputHandler &input, int camW, int camH, const std::string &initialLevel) :
  mCommands(input),
  mLevel(),
  mNextLevel(),
  mCamera(0.f, 0.f, static_cast<float>(camW), static_cast<float>(camH)),
  mGameTime(0),
  mDisplay(display)
{
  display.setCameraSize(camW, camH);
  currentGame = this;

  if (initialLevel.empty())
    game::error("GameState: cannot start, because no level file was given.");

  mLevel = Level::loadFromTmx(initialLevel, mDisplay);
  mLevel->start();
}


void GameState::update(uint32_t step)
{
  if (!mNextLevel.empty()) {
    mLevel = Level::loadFromTmx(mNextLevel, mDisplay);
    mLevel->start();
    mNextLevel.clear();
  }

  mGameTime += step;
  mLevel->update(*this, step);
  mCamera.update(step, *this);

  // Reload the level if the hero dies
  if (!mLevel->getHero())
    loadLevel(mLevel->getFilename());
}

void GameState::draw(Display &target) const
{
  mLevel->draw(target, *this);
}

void GameState::loadLevel(const std::string &levelFile)
{
  mNextLevel = levelFile;
}

GameCommands &GameState::getCommands()
{
  return mCommands;
}

const GameCommands &GameState::getCommands() const
{
  return mCommands;
}

Level &GameState::getLevel()
{
  return *mLevel;
}

const Level &GameState::getLevel() const
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
