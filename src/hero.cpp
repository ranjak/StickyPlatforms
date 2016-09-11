#include "hero.h"
#include "airstate.h"
#include "groundstate.h"
#include "world/tile.h"
#include <algorithm>

namespace game {


Hero::Hero() :
  Entity(0, 0, Tile::SIZE, Tile::SIZE),
  mState(new AirState(*this)),
  mVelocity(0.f, 0.f),
  mOnGround(false),
  mCube(Tile::SIZE, Tile::SIZE)
{
  mCube.setColor(game::GREEN);
}

void Hero::update(uint32_t step, GameState &game)
{
  mState->update(step, game);

  updatePhysics(step, game);
}

void Hero::draw(Display& target) const
{
  mCube.draw(target, mBoundingBox.x, mBoundingBox.y);
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
  Vector<float> destination(mBoundingBox.x + mVelocity.x * step / 1000.f, mBoundingBox.y + mVelocity.y * step / 1000.f);

  // Collision handling

  game.getLevel().tryMoving(*this, destination);

  // If we find out we're not currently on the ground, we'll switch to air state
  bool onGround = game.getLevel().isOnGround(*this);

  // Check collisions
  for (CollisionManifold& col : game.getLevel().checkCollisions(*this)) {

    // Nullify velocity if it's opposite to the normal
    if (mVelocity.x * col.normal.x < 0)
      mVelocity.x = 0;
    else if (mVelocity.y * col.normal.y < 0)
      mVelocity.y = 0;

    // Collision from above (= on ground)
    if (col.normal.y == -1) {
      onGround = true;
      // Place the bounding box correctly (we don't want them to overlap)
      mBoundingBox.y = col.collider.getBoundingBox().y - mBoundingBox.h;
    }
    // From below
    else if (col.normal.y == 1)
      mBoundingBox.y = col.collider.getBoundingBox().y + col.collider.getBoundingBox().h;

    // From the side
    else if (col.normal.x == -1)
      mBoundingBox.x = col.collider.getBoundingBox().x - mBoundingBox.w;

    else if (col.normal.x == 1)
      mBoundingBox.x = col.collider.getBoundingBox().x + col.collider.getBoundingBox().w;
  }

  // Change state if we went from ground to air (and vice versa)
  if (!onGround && mOnGround)
    mState.reset(new AirState(*this));
  else if (onGround && !mOnGround)
    mState.reset(new GroundState(*this));

  mOnGround = onGround;
}

} // namespace game
