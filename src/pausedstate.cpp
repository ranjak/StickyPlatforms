#include "pausedstate.h"
#include "game.h"
#include "uipanel.h"
#include "textwidget.h"
#include "gamecommands.h"
#include "playingstate.h"

namespace game {

PausedState::PausedState(Game &game) :
  mPauseWidget("pauseText")
{
  game.getUI().addWidget<TextWidget>(game.getDisplay(), mPauseWidget, "PAUSED", 64);
  game.getUI().setCentered(mPauseWidget);
  game.getUI().getByName(mPauseWidget)->setHidden(true);
}

void PausedState::update(uint32_t step)
{
  if (Game::current().getCommands().isHit(Command::PAUSE))
    Game::current().setState<PlayingState>();
}

void PausedState::enter()
{
  Game::current().getUI().getByName(mPauseWidget)->setHidden(false);
}

void PausedState::exit()
{
  Game::current().getUI().getByName(mPauseWidget)->setHidden(true);
}

} // namespace game
