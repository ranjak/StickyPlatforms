#ifndef PAUSEDSTATE_H
#define PAUSEDSTATE_H

#include "gamestate.h"
#include <string>

namespace game {

class Game;

class PausedState : public GameState
{
public:
  PausedState(Game &game);

  void update(uint32_t step) override;

  void enter();

  void exit() override;

private:
  const std::string mPauseWidget;
};

} // namespace game

#endif // PAUSEDSTATE_H
