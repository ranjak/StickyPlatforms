#ifndef TRIGGERTYPES_H
#define TRIGGERTYPES_H

#include "triggercomponent.h"
#include <string>
#include <memory>
#include <map>

namespace TMX {
struct PropertyMap;
}

namespace game {

/**
 * @brief Trigger the player's victory, ending the curent level,
 * and possibly loading a new one.
 */
class VictoryTrigger : public TriggerBehavior
{
public:
  VictoryTrigger(const std::string &nextLevel="") : mNextLevel(nextLevel) {}

  void execute(Entity &entity) override;

private:
  std::string mNextLevel;
};

/**
 * @brief Set another object's state (enabled/disabled).
 */
class EnableTrigger : public TriggerBehavior
{
public:
  EnableTrigger(const std::string &entityName, bool enabled) : mEntity(entityName), mEntityState(enabled) {}

  void execute(Entity &entity) override;

private:
  std::string mEntity;
  bool mEntityState;
};


std::unique_ptr<TriggerBehavior> makeTrigger(const std::string &type, const TMX::PropertyMap &properties);

} // namespace game

#endif // TRIGGERTYPES_H
