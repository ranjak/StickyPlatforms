#ifndef FALLSTATE_H
#define FALLSTATE_H

#include "actorstate.h"

namespace game {


/**
 * @brief State in which the actor cannot be controlled.
 * Typically triggered when the actor is hurt or misses a climbing animation.
 * Switches to regular AirState after a given amount of time.
 */
class FallState : public ActorState
{
public:
  FallState(ActorControlComponent &stateMachine);

  void enter() override;

  void update(uint32_t step, GameState &game) override;

private:
  std::uint32_t mEndTimestamp;
};

} // namespace game

#endif // FALLSTATE_H
