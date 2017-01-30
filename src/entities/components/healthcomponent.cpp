#include "healthcomponent.h"
#include "movingphysicscomponent.h"
#include "collision.h"
#include "damagecomponent.h"
#include "graphicscomponent.h"
#include "game.h"
#include "healthbar.h"
#include "damagemsg.h"
#include "stormancerconnection.h"
#include <algorithm>

namespace game {

HealthComponent::HealthComponent(Entity &owner, int hp, int maxHp) :
  mEntity(owner),
  mMaxHP(maxHp),
  mHealthPoints(hp),
  mInvincibilityEnd(0),
  mUI(nullptr)
{}

HealthComponent::HealthComponent(Entity &owner, int hp) :
  HealthComponent(owner, hp, hp)
{}

void HealthComponent::receiveMessageDelegate(Message &msg)
{
  if (msg.type == Message::OnCollision) {

    Collision &col = static_cast<Collision &>(msg);

    if (col.entity != Entity::none) {

      Entity *other = Game::current().getLevel().entities().getEntity(col.entity);

      if (!other || other->isDead())
        return;

      DamageComponent *damage = other->getComponent<DamageComponent>();

      if (damage && (damage->target() & mEntity.group) && (damage->ignoresInvincibility() || Game::current().now() > mInvincibilityEnd)) {

        mHealthPoints = std::max(mHealthPoints - damage->points(), 0);

        // TODO decouple if the UI gets richer
        if (mUI)
          mUI->setHealth(mHealthPoints, mMaxHP);

        if (mEntity.sendsNetworkMessages())
          Game::current().network().updateHealth(mHealthPoints);

        if (mHealthPoints == 0) {
          mEntity.kill();
          return;
        }

        // Make the entity back off
        MovingPhysicsComponent *physics = mEntity.getComponent<MovingPhysicsComponent>();
        if (physics) {
          physics->velocity().x = ((damage->direction) ? damage->direction : col.normal.x) * 300.f * damage->knockback;
          physics->velocity().y = -500.f * damage->knockback;
        }

        GraphicsComponent *graphics = mEntity.getComponent<GraphicsComponent>();
        if (graphics)
          graphics->setBlinking(1.f);

        mInvincibilityEnd = Game::current().now() + 1000;

        mEntity.sendMessage(std::make_unique<DamageMsg>(damage->points(), col.entity));
      }
    }
  }
}

void HealthComponent::setUI(HealthBar *ui)
{
  mUI = ui;
  ui->setHealth(mHealthPoints, mMaxHP);
}

void HealthComponent::setHealthPoints(int hp)
{
  mHealthPoints = hp;

  if (mUI)
    mUI->setHealth(hp, mMaxHP);
}

} // namespace game
