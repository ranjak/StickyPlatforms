#include "gameclearedstate.h"
#include "game.h"
#include "display.h"
#include "textwidget.h"
#include "gamevector.h"
#include "panelcontainer.h"
#include "gamecommands.h"
#include "mainloop.h"
#include "playingstate.h"
#include "util.h"
#include <cstdint>

namespace game {

namespace {

UIPanel makeLevelTimeBoard(Game &game, float width, float height)
{
  UIPanel panel(Vector<float>(width, height));
  const std::vector<std::uint32_t> &times = game.getLevelTimes();

  // Calculate font size, taking padding into account
  int fontHeight = static_cast<int>(height / (times.size() + (times.size()-1)/2.f));
  std::uint32_t initialTime = 0;
  float textPosY = 0.f;

  for (size_t i=0; i<times.size(); i++) {
    std::string widgetName = "time" + std::to_string(i);
    std::string widgetText = "Level " + std::to_string(i+1) + ": " + formatTime(times[i] - initialTime);

    std::unique_ptr<TextWidget> levelTime(std::make_unique<TextWidget>(game.getDisplay(), widgetName, widgetText, fontHeight));
    levelTime->setPosition(width/2.f - levelTime->getSize().x/2.f, textPosY);

    panel.addWidget(std::move(levelTime));
    initialTime = times[i];
    textPosY += fontHeight * 1.5f;
  }

  return panel;
}

}

GameClearedState::GameClearedState(Game &game) :
  mPanelName("clearedPanel"),
  mGame(game)
{
  UIPanel panel(game.getDisplay().getWindowSize());
  const Vector<float> &size = panel.getSize();

  std::unique_ptr<TextWidget> cleared(std::make_unique<TextWidget>(game.getDisplay(), "cleared", "GAME CLEARED!", 64));
  std::unique_ptr<TextWidget> time(std::make_unique<TextWidget>(game.getDisplay(), "totalTime", "Your time: 00:00:00", 48));
  std::unique_ptr<TextWidget> prompt1(std::make_unique<TextWidget>(game.getDisplay(), "prompt1", "Press Backspace to start over", 48));
  std::unique_ptr<TextWidget> prompt2(std::make_unique<TextWidget>(game.getDisplay(), "prompt2", "Press Escape to quit", 48));

  prompt1->setPosition(size.x/2.f - prompt1->getSize().x/2.f, size.y*2.f/3.f - prompt1->getSize().y*1.5f);
  prompt2->setPosition(size.x/2.f - prompt2->getSize().x/2.f, size.y*2.f/3.f + prompt2->getSize().y/2.f);
  cleared->setPosition(size.x/2.f - cleared->getSize().x/2.f, size.y*1.f/3.f - cleared->getSize().y/2.f);
  time->setPosition(size.x/2.f - time->getSize().x/2.f, cleared->getPosition().y + cleared->getSize().y*1.5f);

  panel.addWidget(std::move(cleared));
  panel.addWidget(std::move(prompt1));
  panel.addWidget(std::move(prompt2));
  panel.addWidget(std::move(time));

  game.getUI().addWidget<PanelContainer>(std::move(panel), Vector<float>{0.f, 0.f}, mPanelName, true);
}

void GameClearedState::handleInput(GameCommands &commands)
{
  if (commands.isHeld(Command::PAUSE)) {
    MainLoop::requestExit();
  }
  else if (commands.isHeld(Command::RESET)) {
    mGame.reset();
  }
  else {
    GameState::handleInput(commands);
  }
}

void GameClearedState::enter()
{
  mGame.addLevelTime();

  PanelContainer *panel = static_cast<PanelContainer *>(mGame.getUI().getByName(mPanelName));
  TextWidget *time = static_cast<TextWidget *>(panel->panel().getByName("totalTime"));
  TextWidget *prompt1 = static_cast<TextWidget *>(panel->panel().getByName("prompt1"));

  time->setText(std::string("Your time: ")+formatTime(mGame.now()));

  panel->panel().addWidget<PanelContainer>(
        makeLevelTimeBoard(mGame, panel->getSize().x, prompt1->getPosition().y-10.f - (time->getPosition().y + time->getSize().y + 10.f)),
        Vector<float>(0.f, time->getPosition().y + time->getSize().y + 10.f),
        "timeBoard"
        );

  panel->setHidden(false);
}

void GameClearedState::exit()
{
  PanelContainer *panel = static_cast<PanelContainer *>(mGame.getUI().getByName(mPanelName));

  panel->panel().removeWidget("timeBoard");
  panel->setHidden(true);
}

} // namespace game
