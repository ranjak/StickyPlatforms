#include "game.h"
#include "display.h"
#include "test/test.h"
#include "world/level.h"
#include "log.h"
#include "healthbar.h"
#include "textwidget.h"
#include "entity.h"
#include "healthcomponent.h"
#include "gamevector.h"
#include "color.h"
#include "make_unique.h"
#include "playingstate.h"
#include "loadlevelstate.h"
#include "gameclearedstate.h"
#include <algorithm>
#include <stdexcept>

namespace game {

const std::uint32_t TIMESTEP = 10;

Game *Game::currentGame = nullptr;

Game &Game::current()
{
  return *currentGame;
}

Game::Game(Display &display, InputHandler &input, int camW, int camH, const std::string &initialLevel) :
  mCommands(input),
  mDisplay(display),
  mLevel(),
  mNextLevel(),
  mInitialLevel(initialLevel),
  mCamera(0.f, 0.f, static_cast<float>(camW), static_cast<float>(camH)),
  mUI(display.getWindowSize()),
  mStates {
    std::make_unique<PlayingState>(*this),
    std::make_unique<LoadLevelState>(*this, display),
    std::make_unique<GameClearedState>(*this)
  },
  mState(mStates[State::PLAYING].get())
{
  currentGame = this;

  mUI.addWidget<HealthBar>(Rect<float>(20.f, 20.f, 150.f, 30.f));

  if (initialLevel.empty())
    game::error("GameState: cannot start, because no level file was given.");

  changeLevel(initialLevel);
}


void Game::update(uint32_t step)
{
  if (!mNextLevel.empty()) {
    loadLevel(mNextLevel);
    mNextLevel.clear();
  }

  mState->update(step);
}

void Game::draw(Display &target)
{
  if (mLevel) {
    target.setLogicalSize(mCamera.getViewport().w, mCamera.getViewport().h);

    mLevel->draw(target, *this);
  }

  mUI.draw(target);
}

void Game::changeLevel(const std::string &levelFile)
{
  mNextLevel = levelFile;
}

void Game::loadLevel(const std::string &levelFile)
{
  mLevel = Level::loadFromTmx(levelFile, mDisplay);
  mLevel->start();

  mLevel->getHero()->getComponent<HealthComponent>()->setUI(static_cast<HealthBar *>(mUI.getByName("health")));
}

const std::string &Game::getInitialLevel() const
{
  return mInitialLevel;
}

GameCommands &Game::getCommands()
{
  return mCommands;
}

const GameCommands &Game::getCommands() const
{
  return mCommands;
}

Level &Game::getLevel()
{
  return *mLevel;
}

const Level &Game::getLevel() const
{
  return *mLevel;
}

Camera &Game::getCamera()
{
  return mCamera;
}

const Camera &Game::getCamera() const
{
  return mCamera;
}

UIPanel &Game::getUI()
{
  return mUI;
}

Display &Game::getDisplay()
{
  return mDisplay;
}

uint32_t Game::now() const
{
  return static_cast<PlayingState &>(*mStates[State::PLAYING]).now();
}

} //namespace game
