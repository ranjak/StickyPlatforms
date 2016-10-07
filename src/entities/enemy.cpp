#include "enemy.h"
#include "rectangle.h"
#include "color.h"
#include "world/level.h"
#include "gamestate.h"
#include "basicaicomponent.h"
#include "actorcontrolcomponent.h"
#include "physicscomponent.h"
#include "make_unique.h"
#include <cmath>

namespace game {


Enemy::Enemy(int x, int y, int w, int h, const std::string &name) :
  Entity(x, y, w, h, true, name, std::unique_ptr<Graphics>(new Rectangle(w, h, Color::RED))),
  mSpeed(75.f),
  mHealthPoints(3),
  mInvincibilityEnd(0)
{
  std::unique_ptr<InputComponent> input = std::make_unique<BasicAiComponent>();
  std::unique_ptr<PhysicsComponent> physics = std::make_unique<PhysicsComponent>(*this);
  std::unique_ptr<Component> control = std::make_unique<ActorControlComponent>(*this, *physics, *input, mSpeed);

  addComponent(std::move(input));
  addComponent(std::move(control));
  addComponent(std::move(physics));
}

void Enemy::onCollision(Entity &entity)
{
//  std::uint32_t now = 0;
//  Sword *sword = dynamic_cast<Sword *>(&entity);

//  if (sword != nullptr && (now=GameState::current().now()) > mInvincibilityEnd) {
//    mHealthPoints--;
//    mInvincibilityEnd = now + 200;
//  }
}

bool Enemy::isDead() const
{
  return mHealthPoints <= 0;
}

} // namespace game
