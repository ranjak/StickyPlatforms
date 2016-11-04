#include "movingphysicscomponent.h"
#include "collision.h"
#include "gamevector.h"
#include "game.h"
#include "world/level.h"
#include "make_unique.h"
#include "physicsmanager.h"
#include "entity.h"
#include "log.h"
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
  mIgnoredEntities(),
  mIgnoredTiles()
{
  mEntity.manager().getPhysics().addComponent(this);
}

MovingPhysicsComponent::~MovingPhysicsComponent()
{
  mEntity.manager().getPhysics().removeComponent(this);
}

void MovingPhysicsComponent::updateDelegate(uint32_t step, Game &game)
{
  PhysicsComponent::updateDelegate(step, game);

  if (!mIsOnGround && mHasGravity)
    addAcceleration(gravityAccel);

  // Apply acceleration to speed
  mVelocity.x += mAccelerationSum.x*step / 1000.f;
  mVelocity.y = std::min(FALL_SPEED, mVelocity.y + mAccelerationSum.y*step / 1000.f);

  mIsOnGround = false;

  // Compute new position
  Rect<float> box = mEntity.getGlobalBox();
  // Only use integers for position. Store fractional part in a remainder.
  mRemainder += mVelocity * static_cast<float>(step) / 1000.f;

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

bool MovingPhysicsComponent::isIgnored(EntityID other) const
{
  return std::find(mIgnoredEntities.begin(), mIgnoredEntities.end(), other) != mIgnoredEntities.end();
}

bool MovingPhysicsComponent::isIgnored(const Vector<int> &tile) const
{
  return std::find(mIgnoredTiles.begin(), mIgnoredTiles.end(), tile) != mIgnoredTiles.end();
}

void MovingPhysicsComponent::setIgnored(EntityID other, bool ignored)
{
  auto it = std::find(mIgnoredEntities.begin(), mIgnoredEntities.end(), other);

  if (ignored) {
    if (it != mIgnoredEntities.end())
      Log::getGlobal().get(Log::WARNING) << "MovingPhysicsComponent::setIgnored: cannot set entity "<<other<<" as ignored because it already is"<<std::endl;
    else
      mIgnoredEntities.push_back(other);
  }
  else {
    if (it == mIgnoredEntities.end())
      Log::getGlobal().get(Log::WARNING) << "MovingPhysicsComponent::setIgnored: cannot set entity "<<other<<" as not ignored because is was not set as ignored"<<std::endl;
    else
      mIgnoredEntities.erase(it);
  }
}

void MovingPhysicsComponent::setIgnored(const Vector<int> &tile, bool ignored)
{
  auto it = std::find(mIgnoredTiles.begin(), mIgnoredTiles.end(), tile);

  if (ignored) {
    if (it != mIgnoredTiles.end())
      Log::getGlobal().get(Log::WARNING) << "MovingPhysicsComponent::setIgnored: cannot set tile "<<tile<<" as ignored because it already is"<<std::endl;
    else
      mIgnoredTiles.push_back(tile);
  }
  else {
    if (it == mIgnoredTiles.end())
      Log::getGlobal().get(Log::WARNING) << "MovingPhysicsComponent::setIgnored: cannot set tile "<<tile<<" as not ignored because is was not set as ignored"<<std::endl;
    else
      mIgnoredTiles.erase(it);
  }
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

  auto it = mCollisions.emplace(mCollisions.end(), Entity::none, tile.getCollisionBox(location.x, location.y), normal, tile.isObstacle());

  mEntity.sendMessage(std::make_unique<Collision>(*it));

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
