#include "enemy.h"
#include "rectangle.h"
#include "color.h"
#include "world/level.h"
#include "gamestate.h"
#include "sword.h"
#include <cmath>

namespace game {


Enemy::Enemy(int x, int y, int w, int h, const std::string &name) :
  Entity(x, y, w, h, true, name, std::unique_ptr<Graphics>(new Rectangle(w, h, Color::RED))),
  mSpeed(75.f),
  mMovement(*this),
  mPhysics(mMovement),
  mHealthPoints(3),
  mInvincibilityEnd(0)
{
  // Set initial velocity
  mMovement.velocity().x = - mSpeed;
}

void Enemy::update(uint32_t step, GameState &game)
{
  mPhysics.update(step, game);
  mMovement.update(step, game);
}

void Enemy::onObstacleReached(const Vector<int> &normal)
{
  // Hit something? Go in the opposite direction
  if (normal.x != 0)
    mMovement.velocity().x = mSpeed * normal.x;

  if (normal.y * mMovement.velocity().y < 0)
    mMovement.velocity().y = 0;
}

void Enemy::onCollision(Entity &entity)
{
  std::uint32_t now = 0;
  Sword *sword = dynamic_cast<Sword *>(&entity);

  if (sword != nullptr && (now=GameState::current().now()) > mInvincibilityEnd) {
    mHealthPoints--;
    mInvincibilityEnd = now + 200;
  }
}

bool Enemy::isDead() const
{
  return mHealthPoints <= 0;
}

} // namespace game
