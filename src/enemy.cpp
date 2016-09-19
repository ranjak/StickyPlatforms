#include "enemy.h"
#include "rectangle.h"
#include "color.h"
#include "world/level.h"
#include "gamestate.h"
#include <cmath>

namespace game {


Enemy::Enemy(int x, int y, int w, int h) :
  Entity(x, y, w, h, std::unique_ptr<Graphics>(new Rectangle(w, h, Color::RED))),
  mSpeed(75.f),
  mMovement(*this)
{
  // Set initial velocity
  mMovement.velocity().x = - mSpeed;
}

void Enemy::update(uint32_t step, GameState &game)
{
  mMovement.update(step, game);
}

void Enemy::onObstacleReached(const Vector<int> &normal)
{
  // Hit something? Go in the opposite direction
  if (normal.x != 0)
    mMovement.velocity().x = mSpeed * normal.x;
}

} // namespace game
