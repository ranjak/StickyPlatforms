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
    NONE
  };

  ActorControlComponent(Entity &entity, MovingPhysicsComponent &physics, InputComponent &input, float maxSpeed, float maxAirSpeed);

  void update(uint32_t step, GameState &game) override;
  void receiveMessage(Message &msg) override;
  void setState(State newState);

  MovingPhysicsComponent &physics() { return mPhysics; }
  InputComponent &input() { return mInput; }
  Entity &entity() { return mEntity; }

  float getMaxSpeed() const { return mMaxSpeed; }

private:
  float mMaxSpeed;

  ActorState *mCurrentState;
  State mNextState;
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
};

} // namespace game

#endif // ACTORCONTROLCOMPONENT_H
