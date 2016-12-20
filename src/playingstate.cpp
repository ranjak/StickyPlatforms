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
  if (commands.isHit(Command::PAUSE) || !commands.getInput().applicationHasFocus()) {
    mGame.setState<PausedState>();
  }
  else if (commands.isHeld(Command::RESET)) {
    mGame.reset();
  }
  else {
    GameState::handleInput(commands);
  }
}

void PlayingState::update(uint32_t step)
{
  mGameTime += step;

  mGame.getLevel().update(mGame, step);

  // Reload the level if the hero dies
  if (!mGame.getLevel().getHero())
    mGame.setState<LoadLevelState>(false, mGame.getLevel().getFilename());
}

void PlayingState::reset()
{
  mGameTime = 0;
}

} // namespace game
