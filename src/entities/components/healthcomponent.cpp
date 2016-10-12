#include "healthcomponent.h"
#include "movingphysicscomponent.h"
#include "collisionmsg.h"
#include "damagecomponent.h"
#include "gamestate.h"

namespace game {

HealthComponent::HealthComponent(Entity &owner, int hp) :
  mEntity(owner),
  mHealthPoints(hp),
  mInvincibilityEnd(0)
{

}

void HealthComponent::receiveMessage(Message &msg)
{
  if (msg.type == Message::Collision) {

    CollisionMsg &col = static_cast<CollisionMsg &>(msg);

    if (col.entity != Entity::none) {

      Entity *other = GameState::current().getLevel().entities().getEntity(col.entity);

      if (!other || other->isDead())
        return;

      DamageComponent *damage = other->getComponent<DamageComponent>();

      if (damage && (damage->target() & mEntity.group) && GameState::current().now() > mInvincibilityEnd) {

        mHealthPoints -= damage->points();

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

} // namespace game
