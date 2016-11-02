#ifndef JUMPSTATE_H
#define JUMPSTATE_H

#include "airclingablestate.h"

namespace game {

class JumpState : public AirClingableState
{
public:
  JumpState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed, float friction, float maxJumpHeight=96.f, float impulseTimeFrame=0.15f);

  void enter();

  void update(uint32_t step, GameState &game) override;

  void exit() override;

  float getMaxJumpHeight() const { return mMaxJumpHeight; }

private:
  /** Calculate the impulse speed required in the given time frame to reach the given height */
  float computeInitialSpeed();

private:
  // Maximum height of the jump in pixels
  float mMaxJumpHeight;
  // How long (in seconds) the actor has to hold the Jump key for the jump to reach its max height
  float mImpulseTimeFrame;
  // Vertical speed of the jump, applied during impulse
  float mInitialSpeed;

  std::uint32_t mImpulseEndTimestamp;
};

} // namespace game

#endif // JUMPSTATE_H
