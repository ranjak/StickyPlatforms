#include "hero.h"
#include "airstate.h"
#include "groundstate.h"
#include "world/tile.h"
#include "camera.h"
#include "rectangle.h"
#include "gamestate.h"
#include <algorithm>
#include <cmath>
#include <memory>

namespace game {


Hero::Hero() :
  Entity(0, 0, Tile::SIZE, Tile::SIZE, true, "Hero", std::unique_ptr<Graphics>(new Rectangle(Tile::SIZE, Tile::SIZE, Color::GREEN))),
  mState(new AirState(*this)),
  mNextState(),
  mMovement(*this),
  mPhysics(mMovement),
  mIsSlashing(false),
  mSwordState(*this)
{
}

void Hero::update(uint32_t step, GameState &game)
{
  if (mNextState) {
    mState.reset(mNextState.release());
    mState->enter();
  }

  mState->update(step, game);

  mPhysics.update(step, game);

  mMovement.update(step, game);

  if (mIsSlashing)
    mSwordState.update(step, game);
}

void Hero::onObstacleReached(const Vector<int> &normal)
{
  // Nullify the velocity in the direction of the collision
  if (normal.x != 0)
    mMovement.velocity().x = 0;
  if (normal.y != 0)
    mMovement.velocity().y = 0;
}

void Hero::draw(Display &target, const Camera &camera) const
{
  Entity::draw(target, camera);

  if (mIsSlashing)
    mSwordState.draw(target, camera);
}

void Hero::setState(std::unique_ptr<HeroState> newState)
{
  mNextState = std::move(newState);
}

Vector<float>& Hero::velocity()
{
  return mMovement.velocity();
}

void Hero::swingSword()
{
  // We can't start a new slash before the current one is done.
  if (mIsSlashing)
    return;

  mIsSlashing = true;
  mSwordState.enter();
}

void Hero::stopSword()
{
  mIsSlashing = false;
}

} // namespace game
