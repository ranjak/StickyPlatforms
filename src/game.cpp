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
#include "pausedstate.h"
#include "util.h"
#include <algorithm>
#include <stdexcept>

namespace game {

const std::uint32_t TIMESTEP = 10;

Game *Game::currentGame = nullptr;

Game &Game::current()
{
  return *currentGame;
}

game::Game::Game(Display & display, InputHandler & input, int camW, int camH, const std::string & username, const std::string & scene) :
  mCommands(input),
  mDisplay(display),
  mStormancer(username, scene),
  mLevel(),
  mNextLevel(),
  mInitialLevel(),
  mCamera(0.f, 0.f, static_cast<float>(camW), static_cast<float>(camH)),
  mUI(display.getWindowSize()),
  mStates {
    std::make_unique<PlayingState>(*this),
    std::make_unique<LoadLevelState>(*this, display),
    std::make_unique<GameClearedState>(*this),
    std::make_unique<PausedState>(*this),
  },
  mState(mStates[State::PLAYING].get()),
  mLevelTimes()
{
  currentGame = this;

  // UI setup: health bar and timer
  mUI.addWidget<HealthBar>(Rect<float>(20.f, 20.f, 150.f, 30.f));

  std::string timer = formatTime(0, false);
  mUI.addWidget<TextWidget>(display, "timer", timer, 48);

  TextWidget &timerWidget = *static_cast<TextWidget *>(mUI.getByName("timer"));
  timerWidget.setPosition(mUI.getSize().x - timerWidget.getSize().x - 20.f, 20.f);

  mInitialLevel = mStormancer.getMapFile();

  changeLevel(mInitialLevel);
}


void Game::update(uint32_t step)
{
  if (!mNextLevel.empty()) {
    loadLevel(mNextLevel);
    mNextLevel.clear();
  }

  mState->handleInput(mCommands);
  mState->update(step);

  static_cast<TextWidget *>(mUI.getByName("timer"))->setText(formatTime(currentLevelTime(), false));
}

void Game::draw(Display &target)
{
  if (mLevel) {
    target.setLogicalSize(mCamera.getViewport().w, mCamera.getViewport().h);

    mLevel->draw(target, *this);
  }

  target.setLogicalSize(mUI.getSize().x, mUI.getSize().y);
  mUI.draw(target);
}

void Game::changeLevel(const std::string &levelFile)
{
  if (levelFile.empty()) {
    game::error("No level was specified.");
  }
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

uint32_t Game::currentLevelTime() const
{
  std::uint32_t previousTime = mLevelTimes.empty() ? 0 : mLevelTimes.back();
  return now() - previousTime;
}

const std::vector<uint32_t> &Game::getLevelTimes() const
{
  return mLevelTimes;
}

void Game::addLevelTime()
{
  mLevelTimes.push_back(now());
}

void Game::reset()
{
  mLevelTimes.clear();
  changeLevel(mInitialLevel);
  static_cast<PlayingState &>(*mStates[State::PLAYING]).reset();
  setState<PlayingState>();
}

} //namespace game
