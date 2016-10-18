#ifndef CLIMBINGSTATE_H
#define CLIMBINGSTATE_H

#include "actorstate.h"

namespace game {

class ActorControlComponent;

/**
 * @brief This state is a short animation during which the actor climbs over an edge.
 */
class ClimbingState : public ActorState
{
public:
  ClimbingState(ActorControlComponent& stateMachine);

  void enter() override;

  void update(uint32_t step, GameState &game) override;

  void exit() override;

private:
  int mClimbDirection;
  // Used to check whether the actor has made it over the obstacle
  float mInitialPosX;
};

} // namespace game

#endif // CLIMBINGSTATE_H
