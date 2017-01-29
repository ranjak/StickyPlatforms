#include "connectingstate.h"
#include "game.h"
#include "uipanel.h"
#include "textwidget.h"

namespace game {

ConnectingState::ConnectingState(Game &game) :
  mLoadingWidget("loadingText")
{
  game.getUI().addWidget<TextWidget>(game.getDisplay(), mLoadingWidget, "Connecting...", 64);
  game.getUI().setCentered(mLoadingWidget);
  game.getUI().getByName(mLoadingWidget)->setHidden(true);
}

void ConnectingState::enter()
{
  UIPanel &ui = Game::current().getUI();
  ui.getByName(mLoadingWidget)->setHidden(false);
  ui.getByName("timer")->setHidden(true);
  ui.getByName("health")->setHidden(true);
}

void ConnectingState::exit()
{
  UIPanel &ui = Game::current().getUI();
  ui.getByName(mLoadingWidget)->setHidden(true);
  ui.getByName("timer")->setHidden(false);
  ui.getByName("health")->setHidden(false);
}

} // namespace game
