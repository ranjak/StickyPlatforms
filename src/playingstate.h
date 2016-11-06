#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include "gamestate.h"
#include <cstdint>

namespace game {

class Game;

class PlayingState : public GameState<>
{
public:
  PlayingState(Game &game);

  void update(uint32_t step) override;

  std::uint32_t now() const { return mGameTime; }

private:
  Game &mGame;
  std::uint32_t mGameTime;
};

} // namespace game

#endif // PLAYINGSTATE_H
