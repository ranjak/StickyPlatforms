#include "triggertypes.h"
#include "log.h"
#include "game.h"
#include "make_unique.h"
#include "tmxcommon.h"
#include "entitymanager.h"
#include "triggermsg.h"
#include "cameracontrolmsg.h"
#include "loadlevelstate.h"
#include "gameclearedstate.h"

namespace game {

std::unique_ptr<TriggerBehavior> makeTrigger(const std::string &type, const TMX::PropertyMap &properties, EntityID trigger)
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

  else if (type == "cameraControl") {

    float baseY = -1.f;
    (void) properties.getFloat("triggerCamera_baseY", baseY, -1.f);

    float topMargin = -1.f;
    (void) properties.getFloat("triggerCamera_topMargin", topMargin, -1.f);

    float bottomMargin = -1.f;
    (void) properties.getFloat("triggerCamera_bottomMargin", bottomMargin, -1.f);

    if (baseY < 0.f && topMargin < 0.f && bottomMargin < 0.f)
      Log::getGlobal().get(Log::WARNING) << "TriggerTypes: Camera Control trigger (id="<<trigger<<") doesn't set any parameter" << std::endl;

    return std::make_unique<CameraControlTrigger>(baseY, topMargin, bottomMargin, trigger);
  }

  else {
    Log::getGlobal().get(Log::WARNING) << "TriggerTypes: couldn't create trigger: unknown type: \"" << type << "\""<<std::endl;
  }

  return nullptr;
}


void VictoryTrigger::onEnter(Entity &entity)
{
  if (!mNextLevel.empty()) {
    glog(Log::INFO, "VictoryTrigger: setting level: \""<<mNextLevel<<"\"");
    Game::current().setState<LoadLevelState>(true, mNextLevel);
  }
  else {
    glog(Log::INFO, "VictoryTrigger: game cleared");
    Game::current().setState<GameClearedState>();
  }
}

void EnableTrigger::onEnter(Entity &entity)
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

void CameraControlTrigger::onEnter(Entity &entity)
{
  entity.sendMessage(std::make_unique<TriggerMsg>(TriggerMsg::Type::Enter, mTrigger, std::make_unique<CameraControlMsg>(mBaseY, mTopMargin, mBottomMargin)));
}

void CameraControlTrigger::onExit(Entity *entity)
{
  if (entity)
    entity->sendMessage(std::make_unique<TriggerMsg>(TriggerMsg::Type::Exit, mTrigger));
}

} // namespace game
