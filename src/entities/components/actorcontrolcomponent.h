#ifndef ACTORCONTROLCOMPONENT_H
#define ACTORCONTROLCOMPONENT_H

#include "component.h"
#include "actorstate.h"
#include "groundstate.h"
#include "airclingablestate.h"
#include "clingstate.h"
#include "climbingstate.h"
#include "jumpstate.h"
#include "climbplatformstate.h"
#include "fallstate.h"
#include <vector>

namespace game {

class ActorState;
class InputComponent;
class MovingPhysicsComponent;

class ActorControlComponent : public Component
{
public:
  enum State {
    GROUND,
    AIR,
    AIR_CLINGABLE,
    JUMP,
    CLING,
    CLIMB,
    CLIMB_PLATFORM,
    FALL,
    NONE
  };

  ActorControlComponent(Entity &entity, MovingPhysicsComponent &physics, InputComponent &input, float maxSpeed, float maxAirSpeed);

  void setState(State newState);

  MovingPhysicsComponent &physics() { return mPhysics; }
  InputComponent &input() { return mInput; }
  Entity &entity() { return mEntity; }

  float getMaxSpeed() const { return mMaxSpeed; }

  int getDirection() const;
  void setDirection(int direction);

  float getMaxJumpHeight() const;

private:
  void updateDelegate(uint32_t step, Game &game) override;
  void receiveMessageDelegate(Message &msg) override;

private:
  float mMaxSpeed;
  // Direction the actor is facing. -1=left, 1=right, 0=unknown
  int mDirection;

  ActorState *mCurrentState;
  InputComponent &mInput;
  MovingPhysicsComponent &mPhysics;
  Entity &mEntity;
  GroundState mGroundState;
  AirState mAirState;
  AirClingableState mAirClingableState;
  JumpState mJumpState;
  ClingState mClingState;
  ClimbingState mClimbState;
  ClimbPlatformState mClimbPlatformState;
  FallState mFallState;
};

} // namespace game

#endif // ACTORCONTROLCOMPONENT_H
