#include "triggercomponent.h"
#include "movingphysicscomponent.h"
#include "game.h"
#include "world/level.h"
#include "entitymanager.h"
#include "entity.h"
#include "physicscomponent.h"
#include "collision.h"
#include <vector>
#include <algorithm>

namespace game {


TriggerComponent::TriggerComponent(std::unique_ptr<TriggerBehavior> behavior, PhysicsComponent &physics, bool requiresOnGround, bool singleShot) :
  mBehavior(std::move(behavior)),
  mRequiresOnGround(requiresOnGround),
  mIsSingleShot(singleShot),
  mPhysics(physics),
  mTriggeringEntity(Entity::none)
{

}

void TriggerComponent::updateDelegate(uint32_t step, Game &game)
{
  // Check whether the entity that set up the trigger is still inside
  if (mTriggeringEntity != Entity::none) {

    const std::vector<Collision> &cols = mPhysics.getCollisions();

    if (std::find_if(cols.begin(), cols.end(), [this](const Collision &col) { return col.entity == mTriggeringEntity; }) == cols.end()) {

      mBehavior->onExit(game.getLevel().entities().getEntity(mTriggeringEntity));
      mTriggeringEntity = Entity::none;
    }
  }
}

void TriggerComponent::receiveMessageDelegate(Message &msg)
{
  // Currently, only the hero (so only 1 entity) can set off triggers.
  if (msg.type == Message::OnCollision && mTriggeringEntity == Entity::none) {

    Collision &col = static_cast<Collision &>(msg);

    if (col.entity != Entity::none) {

      Entity *entity = Game::current().getLevel().entities().getEntity(col.entity);

      if (entity && entity->getName() == "Hero" && (!mRequiresOnGround || entity->getComponent<MovingPhysicsComponent>()->isOnGround())) {

        mTriggeringEntity = col.entity;
        mBehavior->onEnter(*entity);

        if (mIsSingleShot)
          setEnabled(false);
      }
    }
  }
  // TODO call onExit when the trigger is destroyed
}

} // namespace game
