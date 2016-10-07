#ifndef ACTORCONTROLCOMPONENT_H
#define ACTORCONTROLCOMPONENT_H

#include "component.h"
#include "actorstate.h"
#include "groundstate.h"
#include "airstate.h"
#include <vector>

namespace game {

class ActorState;
class InputComponent;
class PhysicsComponent;

class ActorControlComponent : public Component
{
public:
  enum State {
    GROUND,
    AIR,
    NONE
  };

  ActorControlComponent(Entity &entity, PhysicsComponent &physics, InputComponent &input, float maxSpeed);

  void update(uint32_t step, GameState &game) override;
  void receiveMessage(Message &msg) override;
  void setState(State newState);

  PhysicsComponent &physics() { return mPhysics; }
  InputComponent &input() { return mInput; }

private:
  ActorState *mCurrentState;
  State mNextState;
  InputComponent &mInput;
  PhysicsComponent &mPhysics;
  Entity &mEntity;
  GroundState mGroundState;
  AirState mAirState;
};

} // namespace game

#endif // ACTORCONTROLCOMPONENT_H
