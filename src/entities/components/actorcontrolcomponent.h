#ifndef ACTORCONTROLCOMPONENT_H
#define ACTORCONTROLCOMPONENT_H

#include "component.h"
#include "actorstate.h"
#include <vector>
#include <memory>

namespace game {

class ActorState;
class InputComponent;
class PhysicsComponent;

class ActorControlComponent : public Component
{
public:
  ActorControlComponent(Entity &entity, PhysicsComponent &physics, InputComponent &input);

  void update(uint32_t step, GameState &game) override;
  void receiveMessage(Message &msg) override;
  void setState(std::unique_ptr<ActorState> newState);

  PhysicsComponent &physics() { return mPhysics; }
  InputComponent &input() { return mInput; }

private:
  std::unique_ptr<ActorState> mCurrentState;
  std::unique_ptr<ActorState> mNextState;
  InputComponent &mInput;
  PhysicsComponent &mPhysics;
  Entity &mEntity;
};

} // namespace game

#endif // ACTORCONTROLCOMPONENT_H
