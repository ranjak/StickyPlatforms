#include "movingphysicscomponent.h"
#include "collision.h"
#include "gamevector.h"
#include "gamestate.h"
#include "world/level.h"
#include "make_unique.h"
#include "physicsmanager.h"
#include "entity.h"
#include <cmath>
#include <algorithm>

namespace game {

const float MovingPhysicsComponent::GRAVITY = 3200.f;

const float MovingPhysicsComponent::FALL_SPEED = 1000.f;

static const Vector<float> gravityAccel(0.f, MovingPhysicsComponent::GRAVITY);

MovingPhysicsComponent::MovingPhysicsComponent(Entity &owner, bool isObstacle, bool hasGravity) :
  PhysicsComponent(owner, isObstacle),
  mVelocity(0.f, 0.f),
  mRemainder(0.f, 0.f),
  mAccelerationSum(0.f, 0.f),
  mIsOnGround(false),
  mHasGravity(hasGravity),
  mIgnoresObstacles(false),
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
  PhysicsComponent::update(step, game);

  if (!mIsOnGround && mHasGravity)
    addAcceleration(gravityAccel);

  // Apply acceleration to speed
  mVelocity.x += mAccelerationSum.x*step / 1000.f;
  mVelocity.y = std::min(FALL_SPEED, mVelocity.y + mAccelerationSum.y*step / 1000.f);

  mCollidingTiles.clear();
  mIsOnGround = false;

  // Compute new position
  Rect<float> box = mEntity.getGlobalBox();
  // Only use integers for position. Store fractional part in a remainder.
  mRemainder += mVelocity * step / 1000.f;

  Vector<float> destination(0.f, 0.f);
  mRemainder.x = std::modf(mRemainder.x, &destination.x);
  mRemainder.y = std::modf(mRemainder.y, &destination.y);

  destination.x += box.x;
  destination.y += box.y;

  mEntity.manager().getPhysics().moveObject(this, destination);
  mEntity.manager().getPhysics().checkCollisions(this);

  mAccelerationSum.x = 0.f;
  mAccelerationSum.y = 0.f;
}

void MovingPhysicsComponent::addAcceleration(const Vector<float> &acceleration)
{
  mAccelerationSum += acceleration;
}

bool MovingPhysicsComponent::isOnGround()
{
  return mIsOnGround;
}

void MovingPhysicsComponent::setGravityEnabled(bool enabled)
{
  mHasGravity = enabled;
}

void MovingPhysicsComponent::setIgnoresObstacles(bool ignore)
{
  mIgnoresObstacles = ignore;
}

const std::vector<std::pair<Vector<int>, Vector<int> > > &MovingPhysicsComponent::getCollidingTiles() const
{
  return mCollidingTiles;
}

void MovingPhysicsComponent::collide(PhysicsComponent &other)
{
  PhysicsComponent::collide(other);

  if (other.isObstacle() && !mIgnoresObstacles)
    collisionResponse(mEntity.getGlobalBox().getCollisionNormal(other.entity().getGlobalBox()));
}

void MovingPhysicsComponent::collide(Tile &tile, const Vector<int> &location)
{
  Vector<int> normal = mEntity.getGlobalBox().getCollisionNormal(tile.getCollisionBox(location.x, location.y));

  mCollidingTiles.push_back(std::make_pair(location, normal));

  mEntity.sendMessage(std::make_unique<Collision>(Entity::none, tile.getCollisionBox(location), normal, tile.isObstacle()));

  if (tile.isObstacle() && !mIgnoresObstacles)
    collisionResponse(normal);
}

void MovingPhysicsComponent::collisionResponse(const Vector<int> &normal)
{
  if (normal.x != 0)
    mVelocity.x /= 2.f;

  if (normal.y != 0)
    mVelocity.y /= 2.f;

  mIsOnGround = mIsOnGround || normal.y < 0;
}

} // namespace game
