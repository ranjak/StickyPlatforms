#include "gravitycomponent.h"
#include "movingphysicscomponent.h"
#include "gamevector.h"

namespace game {

GravityComponent::GravityComponent(MovingPhysicsComponent &physics, float gravity, float fallSpeed) :
  mPhysics(physics),
  mGravity(gravity),
  mFallSpeed(fallSpeed)
{

}

void GravityComponent::update(std::uint32_t step, GameState &game)
{
  Vector<float> &velocity = mPhysics.velocity();

  if (velocity.y < mFallSpeed)
    velocity.y += mGravity * step / 1000.f;
  else
    velocity.y = mFallSpeed;
}

} // namespace game
