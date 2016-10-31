#include "triggercomponent.h"
#include "movingphysicscomponent.h"
#include "gamestate.h"
#include "world/level.h"
#include "entitymanager.h"
#include "entity.h"

namespace game {


TriggerComponent::TriggerComponent(std::unique_ptr<TriggerBehavior> behavior, bool requiresOnGround) :
  mBehavior(std::move(behavior)),
  mRequiresOnGround(requiresOnGround)
{

}

void TriggerComponent::receiveMessage(Message &msg)
{
  if (msg.type == Message::OnCollision) {

    Collision &col = static_cast<Collision &>(msg);

    if (col.entity != Entity::none) {

      Entity *entity = GameState::current().getLevel().entities().getEntity(col.entity);

      if (entity && entity->getName() == "Hero" && (!mRequiresOnGround || entity->getComponent<MovingPhysicsComponent>()->isOnGround())) {
        mBehavior->execute(*entity);
      }
    }
  }
}

} // namespace game
