#include "connectingstate.h"
#include "game.h"
#include "uipanel.h"
#include "textwidget.h"

namespace game {

ConnectingState::ConnectingState(Game &game) :
  mLoadingWidget("loadingText")
{
  game.getUI().addWidget<TextWidget>(game.getDisplay(), mLoadingWidget, "LOADING", 64);
  game.getUI().setCentered(mLoadingWidget);
  game.getUI().getByName(mLoadingWidget)->setHidden(true);
}

void ConnectingState::enter()
{
  Game::current().getUI().getByName(mLoadingWidget)->setHidden(false);
}

void ConnectingState::exit()
{
  Game::current().getUI().getByName(mLoadingWidget)->setHidden(true);
}

} // namespace game
