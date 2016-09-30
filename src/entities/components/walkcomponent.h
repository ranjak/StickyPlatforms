#ifndef WALKCOMPONENT_H
#define WALKCOMPONENT_H

#include "component.h"
#include <cstdint>

namespace game {

class GameState;
class MovementComponent;

class WalkComponent : public Component
{
public:
  WalkComponent(MovementComponent &movement);

  void update(std::uint32_t step, GameState &game) override;

  void receiveMessage(Message &msg) override;

  void setDirection(int direction);

  void setAcceleration(float acceleration);

  void setMaxSpeed(float maxSpeed);

  MovementComponent &getMovement() { return mMovement; }

private:
  MovementComponent &mMovement;
  float mAcceleration;
  float mMaxSpeed;
  int mWalkDirection;
};

} // namespace game

#endif // WALKCOMPONENT_H
