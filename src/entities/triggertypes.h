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

class VictoryTrigger : public TriggerBehavior
{
public:
  VictoryTrigger(const std::string &nextLevel="") : mNextLevel(nextLevel) {}

  void execute(Entity &entity) override;

private:
  std::string mNextLevel;
};


std::unique_ptr<TriggerBehavior> makeTrigger(const std::string &type, const TMX::PropertyMap &properties);

} // namespace game

#endif // TRIGGERTYPES_H
