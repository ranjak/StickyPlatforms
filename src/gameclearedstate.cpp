#include "gameclearedstate.h"
#include "game.h"
#include "display.h"
#include "textwidget.h"
#include "gamevector.h"
#include "panelcontainer.h"
#include "gamecommands.h"
#include "mainloop.h"
#include "playingstate.h"

namespace game {

GameClearedState::GameClearedState(Game &game) :
  mPanelName("clearedPanel"),
  mGame(game)
{
  UIPanel panel(game.getDisplay().getWindowSize());
  const Vector<float> &size = panel.getSize();

  std::unique_ptr<TextWidget> cleared(std::make_unique<TextWidget>(game.getDisplay(), "cleared", "GAME CLEARED!", 64));
  std::unique_ptr<TextWidget> prompt1(std::make_unique<TextWidget>(game.getDisplay(), "prompt1", "Press Backspace to start over", 48));
  std::unique_ptr<TextWidget> prompt2(std::make_unique<TextWidget>(game.getDisplay(), "prompt2", "Press Escape to quit", 48));

  prompt1->setPosition(size.x/2.f - prompt1->getSize().x/2.f, size.y*2.f/3.f - prompt1->getSize().y*1.5f);
  prompt2->setPosition(size.x/2.f - prompt2->getSize().x/2.f, size.y*2.f/3.f + prompt2->getSize().y/2.f);
  cleared->setPosition(size.x/2.f - cleared->getSize().x/2.f, size.y*1.f/3.f - cleared->getSize().y/2.f);

  panel.addWidget(std::move(cleared));
  panel.addWidget(std::move(prompt1));
  panel.addWidget(std::move(prompt2));

  game.getUI().addWidget<PanelContainer>(std::move(panel), Vector<float>{0.f, 0.f}, mPanelName, true);
}

void GameClearedState::update(uint32_t step)
{
  if (mGame.getCommands().isHeld(Command::PAUSE))
    MainLoop::get().requestExit();

  else if (mGame.getCommands().isHeld(Command::RESET)) {
    mGame.loadLevel(mGame.getInitialLevel());
    mGame.setState<PlayingState>();
  }
}

void GameClearedState::enter()
{
  mGame.getUI().getByName(mPanelName)->setHidden(false);
}

void GameClearedState::exit()
{
  mGame.getUI().getByName(mPanelName)->setHidden(true);
}

} // namespace game
