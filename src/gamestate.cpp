#include "gamestate.h"
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

  loadLevel(initialLevel);
}


void GameState::update(uint32_t step)
{
  if (!mNextLevel.empty()) {
    loadLevel(mNextLevel);
    mNextLevel.clear();
  }

  switch (mState) {

  case State::PLAYING:

    mGameTime += step;
    mLevel->update(*this, step);
    mCamera.update(step, *this);

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

void GameState::draw(Display &target) const
{
  target.setLogicalSize(mCamera.getViewport().w, mCamera.getViewport().h);

  mLevel->draw(target, *this);
  mUI.draw(target);
}

void GameState::setPlayingState()
{
  mState = State::PLAYING;
}

void GameState::setLoadingState(bool victory, const std::string &nextLevel)
{
  mState = State::LOADING;
  mLoadingState.enter(victory, (victory && !nextLevel.empty()) ? nextLevel : mLevel->getFilename());
}

void GameState::changeLevel(const std::string &levelFile)
{
  mNextLevel = levelFile;
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

UIPanel &GameState::getUI()
{
  return mUI;
}

uint32_t GameState::now() const
{
  return mGameTime;
}

} //namespace game
