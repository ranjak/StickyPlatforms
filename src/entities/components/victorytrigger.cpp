#include "victorytrigger.h"
#include "collision.h"
#include "entity.h"
#include "gamestate.h"
#include "movingphysicscomponent.h"
#include "log.h"

namespace game {

VictoryTrigger::VictoryTrigger(const std::string &nextLevel) :
  mNextLevel(nextLevel)
{

}

void VictoryTrigger::receiveMessage(Message &msg)
{
  if (msg.type == Message::OnCollision) {

    Collision &col = static_cast<Collision &>(msg);

    if (col.entity != Entity::none) {

      Entity *entity = GameState::current().getLevel().entities().getEntity(col.entity);

      if (entity && entity->getName() == "Hero" && entity->getComponent<MovingPhysicsComponent>()->isOnGround()) {

          glog(Log::INFO, "VictoryTrigger: setting level: \""<<mNextLevel<<"\"");
          GameState::current().setLoadingState(true, mNextLevel);
      }
    }
  }
}

} // namespace game
