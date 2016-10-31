#include "triggertypes.h"
#include "log.h"
#include "gamestate.h"
#include "make_unique.h"
#include "log.h"
#include "tmxcommon.h"

namespace game {

void VictoryTrigger::execute(Entity &entity)
{
  glog(Log::INFO, "VictoryTrigger: setting level: \""<<mNextLevel<<"\"");
  GameState::current().setLoadingState(true, mNextLevel);
}

std::unique_ptr<TriggerBehavior> makeTrigger(const std::string &type, const TMX::PropertyMap &properties)
{
  if (type == "victory") {

    // Get the next level filename if any
    std::string nextLevel = "";
    (void) properties.getString("nextLevel", nextLevel, "");

    return std::make_unique<VictoryTrigger>(nextLevel);
  }
  else {
    Log::getGlobal().get(Log::WARNING) << "TriggerTypes: couldn't create trigger: unknown type: \"" << type << "\""<<std::endl;
  }

  return nullptr;
}

} // namespace game
