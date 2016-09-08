#include "herophysics.h"

namespace game {


HeroPhysics::HeroPhysics(Hero &hero) :
  mBoundingBox(),
  mVelocity(),
  mHero(hero)
{

}

void HeroPhysics::update(std::uint32_t step, GameState &game)
{
  // Move according to velocity
  mBoundingBox.x += mVelocity.x * step / 1000.f;
  mBoundingBox.y += mVelocity.y * step / 1000.f;

  // Collision handling
  // If we find out we're not currently on the ground, we'll switch to air state
  // First check the base ground level. FIXME: do not hardcode
  bool onGround = mBoundingBox.y + mBoundingBox.h >= 240;
  if (onGround) {
    mBoundingBox.y = 240 - mBoundingBox.h;
    mVelocity.y = 0;
  }
  // Check collisions
  for (CollisionManifold& col : game.checkCollisions(mHero)) {

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

}

}
