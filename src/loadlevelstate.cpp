#include "loadlevelstate.h"
#include "uipanel.h"
#include "textwidget.h"
#include "game.h"

namespace game {

const std::string LoadLevelState::gameOverWidget("gameOver");
const std::string LoadLevelState::victoryWidget("victory");

LoadLevelState::LoadLevelState(Game &game, Display &display) :
  mGame(game),
  mCurrentWidget(nullptr),
  mTimeRemaining(0),
  mNextLevel()
{
  game.getUI().addWidget<TextWidget>(display, gameOverWidget, "TRY AGAIN", 64);
  game.getUI().addWidget<TextWidget>(display, victoryWidget, "WELL DONE!", 64);

  game.getUI().setCentered(gameOverWidget);
  game.getUI().setCentered(victoryWidget);
  game.getUI().getByName(gameOverWidget)->setHidden(true);
  game.getUI().getByName(victoryWidget)->setHidden(true);
}

void LoadLevelState::update(uint32_t step)
{
  mTimeRemaining -= step;

  if (mTimeRemaining < 0) {
    mGame.loadLevel(mNextLevel);
    mGame.getUI().getByName(*mCurrentWidget)->setHidden(true);
    mGame.setPlayingState();
  }
}

void LoadLevelState::enter(bool victory, const std::string &nextLevel)
{
  mNextLevel = nextLevel;
  mCurrentWidget = victory ? &victoryWidget : &gameOverWidget;
  mTimeRemaining = 1000;

  mGame.getUI().getByName(*mCurrentWidget)->setHidden(false);
}

} // namespace game
