#include "loadlevelstate.h"
#include "uipanel.h"
#include "textwidget.h"
#include "game.h"
#include "playingstate.h"
#include "util.h"

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
  game.getUI().addWidget<TextWidget>(display, "levelTime", "Time: 00:00:00", 48);

  TextWidget &levelTime = *static_cast<TextWidget *>(game.getUI().getByName("levelTime"));

  game.getUI().setCentered(gameOverWidget);
  game.getUI().setCentered(victoryWidget);
  levelTime.setPosition(game.getUI().getSize().x / 2.f - levelTime.getSize().x / 2.f, game.getUI().getSize().y / 2.f + 50.f);

  game.getUI().getByName(gameOverWidget)->setHidden(true);
  game.getUI().getByName(victoryWidget)->setHidden(true);
  levelTime.setHidden(true);
}

void LoadLevelState::update(uint32_t step)
{
  mTimeRemaining -= step;

  if (mTimeRemaining < 0) {
    mGame.changeLevel(mNextLevel);
    mGame.setState<PlayingState>();
  }
}

void LoadLevelState::enter(bool victory, const std::string &nextLevel)
{
  mNextLevel = nextLevel;
  mTimeRemaining = 1000;

  if (victory) {
    // Display level clear time
    TextWidget &levelTime = *static_cast<TextWidget *>(mGame.getUI().getByName("levelTime"));
    levelTime.setText(std::string("Time: ")+formatTime(mGame.currentLevelTime()));
    levelTime.setHidden(false);

    mGame.addLevelTime();
    mCurrentWidget = &victoryWidget;
  }
  else {
    mCurrentWidget = &gameOverWidget;
  }

  mGame.getUI().getByName(*mCurrentWidget)->setHidden(false);
}

void LoadLevelState::exit()
{
  mGame.getUI().getByName(*mCurrentWidget)->setHidden(true);
  mGame.getUI().getByName("levelTime")->setHidden(true);
}

} // namespace game
