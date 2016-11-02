#include "triggertypes.h"
#include "log.h"
#include "gamestate.h"
#include "make_unique.h"
#include "tmxcommon.h"
#include "entitymanager.h"

namespace game {

std::unique_ptr<TriggerBehavior> makeTrigger(const std::string &type, const TMX::PropertyMap &properties)

{
  if (type == "victory") {

    // Get the next level filename if any
    std::string nextLevel = "";
    (void) properties.getString("nextLevel", nextLevel, "");

    return std::make_unique<VictoryTrigger>(nextLevel);
  }

  else if (type == "enable") {

    bool enable = true;
    (void) properties.getBool("triggerEnable_setEnabled", enable, true);

    std::string entityName;
    if (!properties.getString("triggerEnable_entity", entityName)) {

      Log::getGlobal().get(Log::WARNING) << "TriggerTypes: couldn't create EnableTrigger: missing string property \"triggerEnable_entity\""<<std::endl;
      return nullptr;
    }

    return std::make_unique<EnableTrigger>(entityName, enable);
  }
  else {
    Log::getGlobal().get(Log::WARNING) << "TriggerTypes: couldn't create trigger: unknown type: \"" << type << "\""<<std::endl;
  }

  return nullptr;
}


void VictoryTrigger::execute(Entity &entity)
{
  glog(Log::INFO, "VictoryTrigger: setting level: \""<<mNextLevel<<"\"");
  GameState::current().setLoadingState(true, mNextLevel);
}

void EnableTrigger::execute(Entity &entity)
{
  Entity *target = entity.manager().getEntity(mEntity);

  if (target) {
    glog(Log::INFO, "EnableTrigger: "<<(mEntityState ? "Enabling " : "Disabling ")<<*target);
    target->setEnabled(mEntityState);
  }
  else {
    glog(Log::WARNING, "EnableTrigger: target entity (name="<<mEntity<<") not found");
  }
}

} // namespace game
