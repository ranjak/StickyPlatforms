#ifndef GAMECLEAREDSTATE_H
#define GAMECLEAREDSTATE_H

#include "gamestate.h"
#include "uipanel.h"

namespace game {

class Game;

class GameClearedState : public GameState
{
public:
  GameClearedState(Game &game);

  void handleInput(GameCommands &commands) override;

  void enter();

  void exit() override;

private:
  // Name of the panel container widget in the game's panel
  std::string mPanelName;
  Game &mGame;
};

} // namespace game

#endif // GAMECLEAREDSTATE_H
