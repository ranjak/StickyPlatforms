#ifndef CONNECTINGSTATE_H
#define CONNECTINGSTATE_H

#include "gamestate.h"
#include <string>

namespace game
{

class Game;

class ConnectingState : public GameState
{
public:
  ConnectingState(Game &game);

  void enter();

  void exit() override;

private:
  std::string mLoadingWidget;
};

} // namespace game
#endif // CONNECTINGSTATE_H
