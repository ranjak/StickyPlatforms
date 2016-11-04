#ifndef TRIGGERCOMPONENT_H
#define TRIGGERCOMPONENT_H

#include "component.h"
#include "entity.h"
#include <memory>

namespace game {

class PhysicsComponent;


class TriggerBehavior
{
public:
  virtual ~TriggerBehavior() {}

  /**
   * @brief onEnter Called when \p entity first collides with the trigger.
   * @param entity The entity that set off the trigger. Currently, this is always the hero.
   */
  virtual void onEnter(Entity &entity) {}

  /**
   * @brief onExit Called when the entity that set off the trigger cease to be in contact with it.
   * @param entity The entity that was previously in the trigger. Could be null,
   * as the reason for it leaving could be that it was destroyed.
   */
  virtual void onExit(Entity *entity) {}
};


/**
 * @brief A component that executes custom code when the entity it belongs to collides
 * with the hero.
 */
class TriggerComponent : public Component
{
public:
  TriggerComponent(std::unique_ptr<TriggerBehavior> behavior, PhysicsComponent &physics, bool requiresOnGround=false, bool singleShot=false);

  void updateDelegate(std::uint32_t step, Game &game) override;

private:
  void receiveMessageDelegate(Message &msg) override;

private:
  std::unique_ptr<TriggerBehavior> mBehavior;
  bool mRequiresOnGround;
  bool mIsSingleShot;
  PhysicsComponent &mPhysics;
  // The entity that is currently inside this trigger. Should be the hero.
  EntityID mTriggeringEntity;
};



} // namespace game

#endif // TRIGGERCOMPONENT_H
