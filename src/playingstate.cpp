#include "playingstate.h"
#include "loadlevelstate.h"
#include "world/level.h"
#include "game.h"
#include "gamecommands.h"
#include "pausedstate.h"

namespace game {

PlayingState::PlayingState(Game &game) :
  mGame(game),
  mGameTime(0)
{

}

void PlayingState::handleInput(GameCommands &commands)
{
  if (commands.isHit(Command::PAUSE))
    mGame.setState<PausedState>();
  else
    GameState::handleInput(commands);
}

void PlayingState::update(uint32_t step)
{
  mGameTime += step;

  mGame.getLevel().update(mGame, step);

  // Reload the level if the hero dies
  if (!mGame.getLevel().getHero())
    mGame.setState<LoadLevelState>(false, mGame.getLevel().getFilename());
}

} // namespace game
