#include "gamestate.h"
#include "gamecommands.h"
#include "game.h"
#include "display.h"

namespace game {

void GameState::handleInput(GameCommands &commands)
{
  if (commands.isHit(Command::TOGGLE_FULLSCREEN))
    Game::current().getDisplay().toggleFullscreen();
}

}
