#include "physicscomponent.h"
#include "obstaclereachedmsg.h"
#include "gamevector.h"
#include "gamestate.h"
#include "world/level.h"
#include <cmath>

namespace game {

PhysicsComponent::PhysicsComponent(Entity &owner) :
  mVelocity(0.f, 0.f),
  mRemainder(0.f, 0.f),
  mEntity(owner),
  mGravity(2500.f),
  mFallSpeed(800.f)
{

}

void PhysicsComponent::update(uint32_t step, GameState &game)
{
  // Apply gravity
  if (mVelocity.y < mFallSpeed)
    mVelocity.y += mGravity * step / 1000.f;
  else
    mVelocity.y = mFallSpeed;

  // Compute new position
  Rect<float> box = mEntity.getGlobalBox();
  // Only use integers for position. Store fractional part in a remainder.
  mRemainder.x += mVelocity.x * step / 1000.f;
  mRemainder.y += mVelocity.y * step / 1000.f;

  Vector<float> destination(0.f, 0.f);
  mRemainder.x = std::modf(mRemainder.x, &destination.x);
  mRemainder.y = std::modf(mRemainder.y, &destination.y);

  destination.x += box.x;
  destination.y += box.y;

  game.getLevel().tryMoving(mEntity, destination);

  if (mEntity.isCollidable())
    game.getLevel().handleCollisions(mEntity);
}

void PhysicsComponent::receiveMessage(Message &message)
{
  switch (message.type) {

  case Message::ObstacleReached:
  {
    const Vector<int> &normal = static_cast<ObstacleReachedMsg &>(message).normal;

    if (normal.x != 0)
      mVelocity.x /= 2.f;

    if (normal.y != 0)
      mVelocity.y /= 2.f;

    break;
  }
  default:
    break;
  }
}

bool PhysicsComponent::isOnGround()
{
  return GameState::current().getLevel().isOnGround(mEntity);
}

} // namespace game
