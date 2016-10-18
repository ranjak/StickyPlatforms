#include "gravitycomponent.h"
#include "movingphysicscomponent.h"
#include "gamevector.h"

namespace game {

GravityComponent::GravityComponent(MovingPhysicsComponent &physics, float gravity, float fallSpeed) :
  mPhysics(physics),
  mGravity(gravity),
  mFallSpeed(fallSpeed),
  mIsEnabled(true)
{

}

void GravityComponent::update(std::uint32_t step, GameState &game)
{
  if (!mIsEnabled)
    return;

  Vector<float> &velocity = mPhysics.velocity();

  if (velocity.y < mFallSpeed)
    velocity.y += mGravity * step / 1000.f;
  else
    velocity.y = mFallSpeed;
}

void GravityComponent::setEnabled(bool enabled)
{
  mIsEnabled = enabled;
}

} // namespace game
