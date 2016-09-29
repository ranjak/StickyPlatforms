#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <cstdint>

namespace game {

class GameState;
class MovementComponent;

class PhysicsComponent
{
public:
  PhysicsComponent(MovementComponent &movement);

  void update(std::uint32_t step, GameState &game);

private:
  float mGravity;
  float mFallSpeed;
  // Component belonging to the owner of this component
  MovementComponent &mMovement;
};

} // namespace game

#endif // PHYSICSCOMPONENT_H
