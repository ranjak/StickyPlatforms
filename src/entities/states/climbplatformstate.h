#ifndef CLIMBPLATFORMSTATE_H
#define CLIMBPLATFORMSTATE_H

#include "actorstate.h"
#include "rect.h"
#include "gamevector.h"
#include "entity.h"
#include <vector>

namespace game {

class Game;

class ClimbPlatformState : public ActorState
{
public:
  ClimbPlatformState(ActorControlComponent &stateMachine);

  void enter() override;

  void update(std::uint32_t step, Game &game) override;

  void exit() override;

  void receiveMessage(Message &msg) override;

private:
  // Tallest platform over which we're climbing
  Rect<float> mPlatform;
  // Obstacles we need to go through in order to climb
  std::vector<EntityID> mEntityObstacles;
  std::vector<Vector<int>> mTileObstacles;
  // If climbing was interrupted (unexpected obstacle)
  bool mFalling;
};

} // namespace game

#endif // CLIMBPLATFORMSTATE_H
