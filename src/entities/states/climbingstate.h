#ifndef CLIMBINGSTATE_H
#define CLIMBINGSTATE_H

#include "actorstate.h"
#include "rect.h"

namespace game {

class ActorControlComponent;
class Message;

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

  void receiveMessage(Message &msg) override;

private:
  int mClimbDirection;
  // Used to check whether the actor has made it over the obstacle
  float mInitialPosX;
  // The solid over which we're climbing
  Rect<float> mEdge;
};

} // namespace game

#endif // CLIMBINGSTATE_H
