#ifndef ACTORCONTROLCOMPONENT_H
#define ACTORCONTROLCOMPONENT_H

#include "component.h"
#include "actorstate.h"
#include "groundstate.h"
#include "airclingablestate.h"
#include "clingstate.h"
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
    CLING,
    NONE
  };

  ActorControlComponent(Entity &entity, MovingPhysicsComponent &physics, InputComponent &input, float maxSpeed);

  void update(uint32_t step, GameState &game) override;
  void receiveMessage(Message &msg) override;
  void setState(State newState);

  MovingPhysicsComponent &physics() { return mPhysics; }
  InputComponent &input() { return mInput; }
  Entity &entity() { return mEntity; }

  int getDirection();

private:
  ActorState *mCurrentState;
  State mNextState;
  InputComponent &mInput;
  MovingPhysicsComponent &mPhysics;
  Entity &mEntity;
  GroundState mGroundState;
  AirState mAirState;
  AirClingableState mAirClingableState;
  ClingState mClingState;
};

} // namespace game

#endif // ACTORCONTROLCOMPONENT_H
