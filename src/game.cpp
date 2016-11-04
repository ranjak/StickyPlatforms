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
  mLevel(),
  mNextLevel(),
  mCamera(0.f, 0.f, static_cast<float>(camW), static_cast<float>(camH)),
  mUI(display.getWindowSize()),
  mGameTime(0),
  mState(State::PLAYING),
  mLoadingState(*this, display),
  mDisplay(display)
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

  switch (mState) {

  case State::PLAYING:

    mGameTime += step;
    mLevel->update(*this, step);

    // Reload the level if the hero dies
    if (!mLevel->getHero())
      setLoadingState(false);

    break;

  case State::LOADING:

    mLoadingState.update(step);
    break;

  case State::VICTORY:
    // TODO
    break;
  }

}

void Game::draw(Display &target)
{
  if (mLevel) {
    target.setLogicalSize(mCamera.getViewport().w, mCamera.getViewport().h);

    mLevel->draw(target, *this);
  }

  mUI.draw(target);
}

void Game::setPlayingState()
{
  mState = State::PLAYING;
}

void Game::setLoadingState(bool victory, const std::string &nextLevel)
{
  mState = State::LOADING;
  mLoadingState.enter(victory, (victory && !nextLevel.empty()) ? nextLevel : mLevel->getFilename());
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
  return mGameTime;
}

} //namespace game
