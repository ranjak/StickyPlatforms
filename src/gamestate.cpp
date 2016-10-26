#include "gamestate.h"
#include "display.h"
#include "test/test.h"
#include "world/level.h"
#include "log.h"
#include "healthbar.h"
#include "entity.h"
#include "healthcomponent.h"
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
  mUI(),
  mGameTime(0),
  mDisplay(display)
{
  display.setCameraSize(camW, camH);
  currentGame = this;

  mUI.addWidget<HealthBar>(Rect<float>(10.f, 10.f, 75.f, 15.f));

  if (initialLevel.empty())
    game::error("GameState: cannot start, because no level file was given.");

  loadLevel(initialLevel);
}


void GameState::update(uint32_t step)
{
  if (!mNextLevel.empty()) {
    loadLevel(mNextLevel);
    mNextLevel.clear();
  }

  mGameTime += step;
  mLevel->update(*this, step);
  mCamera.update(step, *this);

  // Reload the level if the hero dies
  if (!mLevel->getHero())
    mNextLevel = mLevel->getFilename();
}

void GameState::draw(Display &target) const
{
  mLevel->draw(target, *this);
  mUI.draw(target);
}

void GameState::loadLevel(const std::string &levelFile)
{
  mLevel = Level::loadFromTmx(levelFile, mDisplay);
  mLevel->start();

  mLevel->getHero()->getComponent<HealthComponent>()->setUI(static_cast<HealthBar *>(mUI.getByName("health")));
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
