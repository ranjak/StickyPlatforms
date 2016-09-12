#include "hero.h"
#include "airstate.h"
#include "groundstate.h"
#include "world/tile.h"
#include "camera.h"
#include <algorithm>
#include <cmath>

namespace game {


Hero::Hero() :
  Entity(0, 0, Tile::SIZE, Tile::SIZE),
  mState(new AirState(*this)),
  mVelocity(0.f, 0.f),
  mOnGround(false),
  mCube(Tile::SIZE, Tile::SIZE),
  mRemainder(0.f, 0.f)
{
  mCube.setColor(game::GREEN);
}

void Hero::update(uint32_t step, GameState &game)
{
  mState->update(step, game);

  updatePhysics(step, game);
}

void Hero::draw(Display& target, const Camera &cam) const
{
  Vector<float> pos = cam.toCamCoords(getPosition());

  mCube.draw(target, pos.x, pos.y);
}

void Hero::onObstacleReached(const Vector<int> &normal)
{
  // Nullify the velocity in the direction of the collision
  if (normal.x != 0)
    mVelocity.x = 0;
  if (normal.y != 0)
    mVelocity.y = 0;
}

Vector<float>& Hero::velocity()
{
  return mVelocity;
}

void Hero::updatePhysics(uint32_t step, GameState &game)
{
  // Move according to velocity

  // Only use integers for position. Store fractional part in a remainder.
  mRemainder.x += mVelocity.x * step / 1000.f;
  mRemainder.y += mVelocity.y * step / 1000.f;

  Vector<float> destination(0.f, 0.f);
  mRemainder.x = std::modf(mRemainder.x, &destination.x);
  mRemainder.y = std::modf(mRemainder.y, &destination.y);

  destination.x += mBoundingBox.x;
  destination.y += mBoundingBox.y;

  game.getLevel().tryMoving(*this, destination);

  // If we find out we're not currently on the ground, we'll switch to air state
  bool onGround = game.getLevel().isOnGround(*this);

  // Change state if we went from ground to air (and vice versa)
  if (!onGround && mOnGround)
    mState.reset(new AirState(*this));
  else if (onGround && !mOnGround)
    mState.reset(new GroundState(*this));

  mOnGround = onGround;
}

} // namespace game
