#ifndef WALLHUGSTATE_H
#define WALLHUGSTATE_H

#include "actorstate.h"

namespace game {

class ActorControlComponent;
class GameState;

class WallHugState : public ActorState
{
public:
  WallHugState(ActorControlComponent &stateMachine);

  void enter() override;

  void update(std::uint32_t step, GameState &game);

private:
  std::uint32_t mWalljumpTimeFrame;
};

} // namespace game

#endif // WALLHUGSTATE_H
