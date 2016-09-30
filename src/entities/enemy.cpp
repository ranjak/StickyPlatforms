#include "enemy.h"
#include "rectangle.h"
#include "color.h"
#include "world/level.h"
#include "gamestate.h"
#include "sword.h"
#include "basicaicomponent.h"
#include "movementcomponent.h"
#include "physicscomponent.h"
#include "walkcomponent.h"
#include <cmath>

namespace game {


Enemy::Enemy(int x, int y, int w, int h, const std::string &name) :
  Entity(x, y, w, h, true, name, std::unique_ptr<Graphics>(new Rectangle(w, h, Color::RED))),
  mSpeed(75.f),
  mHealthPoints(3),
  mInvincibilityEnd(0)
{
  std::unique_ptr<MovementComponent> mvt(new MovementComponent(*this));
  std::unique_ptr<WalkComponent> walk(new WalkComponent(*mvt));

  walk->setMaxSpeed(mSpeed);
  walk->setAcceleration(2000.f);
  walk->setDirection(-1);

  addComponent(std::unique_ptr<Component>(new BasicAiComponent(*this)));
  addComponent(std::unique_ptr<Component>(new PhysicsComponent(*mvt)));
  addComponent(std::move(walk));
  addComponent(std::move(mvt));
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
