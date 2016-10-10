#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "component.h"
#include "gamevector.h"
#include <cstdint>

namespace game {

class GameState;
class MovementComponent;

class PhysicsComponent : public Component
{
public:
  PhysicsComponent(Entity &owner);

  void update(std::uint32_t step, GameState &game) override;

  void receiveMessage(Message &message) override;

  Vector<float> &velocity() { return mVelocity; }

  bool isOnGround();

  Entity &entity() { return mEntity; }

private:
  Vector<float> mVelocity;
  Vector<float> mRemainder;
  Entity& mEntity;
  float mGravity;
  float mFallSpeed;
};

} // namespace game

#endif // PHYSICSCOMPONENT_H
