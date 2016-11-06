#ifndef GAMECLEAREDSTATE_H
#define GAMECLEAREDSTATE_H

#include "gamestate.h"
#include "uipanel.h"

namespace game {

class Game;

class GameClearedState : GameState<>
{
public:
  GameClearedState(Game &game);

  void update(uint32_t step) override;

  void enter() override;

  void exit() override;

private:
  // Name of the panel container widget in the game's panel
  std::string mPanelName;
  Game &mGame;
};

} // namespace game

#endif // GAMECLEAREDSTATE_H
