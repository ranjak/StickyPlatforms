#include "movingphysicscomponent.h"
#include "collisionmsg.h"
#include "gamevector.h"
#include "gamestate.h"
#include "world/level.h"
#include "make_unique.h"
#include "physicsmanager.h"
#include <cmath>

namespace game {

MovingPhysicsComponent::MovingPhysicsComponent(Entity &owner) :
  PhysicsComponent(owner),
  mVelocity(0.f, 0.f),
  mRemainder(0.f, 0.f),
  mGravity(2500.f),
  mFallSpeed(800.f),
  mIsOnGround(false),
  mCollidingTiles()
{
  mEntity.manager().getPhysics().addComponent(this);
}

MovingPhysicsComponent::~MovingPhysicsComponent()
{
  mEntity.manager().getPhysics().removeComponent(this);
}

void MovingPhysicsComponent::update(uint32_t step, GameState &game)
{
  // Collisions from last update are irrelevant
  mCollidingEntities.clear();
  mCollidingTiles.clear();
  mIsOnGround = false;

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

  mEntity.manager().getPhysics().moveObject(this, destination);

  mEntity.manager().getPhysics().checkCollisions(this);
}

bool MovingPhysicsComponent::isOnGround()
{
  return mIsOnGround;
}

void MovingPhysicsComponent::collide(PhysicsComponent &other)
{
  PhysicsComponent::collide(other);

  if (other.isObstacle())
    collisionResponse(mEntity.getGlobalBox().getCollisionNormal(other.entity().getGlobalBox()));
}

void MovingPhysicsComponent::collide(Tile &tile, const Vector<int> &location)
{
  mCollidingTiles.push_back(location);

  Vector<int> normal = mEntity.getGlobalBox().getCollisionNormal(tile.getCollisionBox(location.x, location.y));

  mEntity.sendMessage(std::make_unique<CollisionMsg>(location, normal));

  if (tile.isObstacle())
    collisionResponse(normal);
}

void MovingPhysicsComponent::collisionResponse(const Vector<int> &normal)
{
  if (normal.x != 0)
    mVelocity.x /= 2.f;

  if (normal.y != 0)
    mVelocity.y /= 2.f;

  if (!mIsOnGround)
    mIsOnGround = normal.y < 0;
}

} // namespace game
