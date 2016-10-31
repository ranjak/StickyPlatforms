#ifndef TRIGGERCOMPONENT_H
#define TRIGGERCOMPONENT_H

#include "component.h"
#include <memory>

namespace game {

class Entity;

class TriggerBehavior
{
public:
  virtual ~TriggerBehavior() {}

  virtual void execute(Entity &entity) = 0;
};


/**
 * @brief A component that executes custom code when the entity it belongs to collides
 * with the hero.
 */
class TriggerComponent : public Component
{
public:
  TriggerComponent(std::unique_ptr<TriggerBehavior> behavior, bool requiresOnGround=false);

  void receiveMessage(Message &msg) override;

private:
  std::unique_ptr<TriggerBehavior> mBehavior;
  bool mRequiresOnGround;
};



} // namespace game

#endif // TRIGGERCOMPONENT_H
