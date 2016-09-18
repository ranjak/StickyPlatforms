#include "enemy.h"
#include "rectangle.h"
#include "color.h"
#include "world/level.h"
#include "gamestate.h"
#include <cmath>

namespace game {


Enemy::Enemy(int x, int y, int w, int h) :
  Entity(x, y, w, h, std::unique_ptr<Graphics>(new Rectangle(w, h, Color::RED))),
  mDirectionX(-1),
  mSpeed(75),
  mRemainder(0.f)
{
}

void Enemy::update(uint32_t step, GameState &game)
{
  // Move along the direction. Currently we're supposed to move along X only.
  mRemainder += mDirectionX * mSpeed * step / 1000.f;

  Vector<float> destination(0.f, mBoundingBox.y);
  mRemainder = std::modf(mRemainder, &destination.x);

  destination.x += mBoundingBox.x;
  game.getLevel().tryMoving(*this, destination);
}

void Enemy::onObstacleReached(const Vector<int> &normal)
{
  // Hit something? Go to the opposite direction
  if (normal.x != 0)
    mDirectionX = normal.x;
}

} // namespace game
