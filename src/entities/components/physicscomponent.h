#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "component.h"
#include <cstdint>

namespace game {

class GameState;
class MovementComponent;

class PhysicsComponent : public Component
{
public:
  PhysicsComponent(MovementComponent &movement);

  void update(std::uint32_t step, GameState &game) override;

  void receiveMessage(Message &message) override;

private:
  float mGravity;
  float mFallSpeed;
  // Component belonging to the owner of this component
  MovementComponent &mMovement;
};

} // namespace game

#endif // PHYSICSCOMPONENT_H
