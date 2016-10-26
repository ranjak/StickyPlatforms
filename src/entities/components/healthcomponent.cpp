#include "healthcomponent.h"
#include "movingphysicscomponent.h"
#include "collision.h"
#include "damagecomponent.h"
#include "gamestate.h"
#include "healthbar.h"

namespace game {

HealthComponent::HealthComponent(Entity &owner, int hp) :
  mEntity(owner),
  mMaxHP(hp),
  mHealthPoints(hp),
  mInvincibilityEnd(0),
  mUI(nullptr)
{

}

void HealthComponent::receiveMessage(Message &msg)
{
  if (msg.type == Message::OnCollision) {

    Collision &col = static_cast<Collision &>(msg);

    if (col.entity != Entity::none) {

      Entity *other = GameState::current().getLevel().entities().getEntity(col.entity);

      if (!other || other->isDead())
        return;

      DamageComponent *damage = other->getComponent<DamageComponent>();

      if (damage && (damage->target() & mEntity.group) && (damage->ignoresInvincibility() || GameState::current().now() > mInvincibilityEnd)) {

        mHealthPoints -= damage->points();

        // TODO decouple if the UI gets richer
        if (mUI)
          mUI->setHealth(mHealthPoints, mMaxHP);

        if (mHealthPoints <= 0) {
          mEntity.kill();
          return;
        }

        // Make the entity back off
        MovingPhysicsComponent *physics = mEntity.getComponent<MovingPhysicsComponent>();
        if (physics) {
          physics->velocity().x = col.normal.x * 1000.f;
          physics->velocity().y = -500.f;
        }

        mInvincibilityEnd = GameState::current().now() + 1000;
      }
    }
  }
}

void HealthComponent::setUI(HealthBar *ui)
{
  mUI = ui;
  ui->setHealth(mHealthPoints, mMaxHP);
}

} // namespace game
