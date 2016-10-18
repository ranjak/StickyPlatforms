#ifndef GRAVITYCOMPONENT_H
#define GRAVITYCOMPONENT_H

#include "component.h"

namespace game {

class MovingPhysicsComponent;

class GravityComponent : public Component
{
public:
  GravityComponent(MovingPhysicsComponent &physics, float gravity=2500.f, float fallSpeed=800.f);

  void update(std::uint32_t step, GameState &game) override;

  void setEnabled(bool enabled);

private:
  MovingPhysicsComponent &mPhysics;
  float mGravity;
  float mFallSpeed;
  bool mIsEnabled;
};

} // namespace game

#endif // GRAVITYCOMPONENT_H
