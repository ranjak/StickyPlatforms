#ifndef CLIMBPLATFORMSTATE_H
#define CLIMBPLATFORMSTATE_H

#include "actorstate.h"
#include "rect.h"

namespace game {

class GameState;

class ClimbPlatformState : public ActorState
{
public:
  ClimbPlatformState(ActorControlComponent &stateMachine);

  void enter() override;

  void update(std::uint32_t step, GameState &game) override;

  void exit() override;

private:
  // Tallest platform over which we're climbing
  Rect<float> mPlatform;
};

} // namespace game

#endif // CLIMBPLATFORMSTATE_H
