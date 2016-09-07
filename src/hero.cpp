#include "hero.h"
#include <algorithm>

namespace game {


float Hero::GRAVITY = 2500.f;

Hero::Hero() :
  Entity(0, 0, 20, 20),
  mFallSpeed(500.f),
  mState(State::AIR),
  mVelocity(0.f, 0.f),
  mCube(0, 0, 20, 20)
{
  mCube.setColor(game::GREEN);
}

void Hero::update(uint32_t step, GameState &game)
{
  mVelocity.x = 0.f;
  // These inputs work for both states
  if (game.isCommandPressed(Command::LEFT))
    mVelocity.x -= 300.f;
  if (game.isCommandPressed(Command::RIGHT))
    mVelocity.x += 300.f;

  mBoundingBox.x += mVelocity.x * step / 1000.f;

  switch (mState) {

  case State::GROUND:
    // Jump: set initial jump speed
    if (game.isCommandPressed((Command::JUMP)))
      mVelocity.y = - 1000;

    mBoundingBox.y += mVelocity.y * step / 1000.f;
    break;

  case State::AIR:
    // Apply gravity (accelerate until max fall speed)
    mVelocity.y = std::min(mFallSpeed, mVelocity.y + (GRAVITY * step / 1000.f));

    mBoundingBox.y += mVelocity.y * step / 1000.f;
    break;
  }

  // Collision handling
  // If we find out we're not currently on the ground, we'll switch to air state
  // First check the base ground level. FIXME: do not hardcode
  bool onGround = mBoundingBox.y + mBoundingBox.h >= 240;
  if (onGround) {
    mBoundingBox.y = 240 - mBoundingBox.h;
    mVelocity.y = 0;
  }
  // Check collisions
  for (CollisionManifold& col : game.checkCollisions(*this)) {

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

  if (!onGround)
    mState = State::AIR;
  else
    mState = State::GROUND;

  // Update the graphics
  // TODO remove duplicate data ?
  mCube.setPos(mBoundingBox.x, mBoundingBox.y);
}

void Hero::draw(Display& target) const
{
  mCube.draw(target);
}

} // namespace game
