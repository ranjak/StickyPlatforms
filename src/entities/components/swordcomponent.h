#ifndef SWORDCOMPONENT_H
#define SWORDCOMPONENT_H

#include "component.h"

namespace game {

class MovingPhysicsComponent;

class SwordComponent : public Component
{
public:
  SwordComponent(MovingPhysicsComponent &physics);

  void swing(int direction);

  Entity &getEntity();

private:
  void updateDelegate(uint32_t step, GameState &game) override;

private:
  MovingPhysicsComponent &mPhysics;
  std::uint32_t mDamageEnd;
  std::uint32_t mSwingEnd;
  std::uint32_t mCooldownEnd;
};

} // namespace game

#endif // SWORDCOMPONENT_H
