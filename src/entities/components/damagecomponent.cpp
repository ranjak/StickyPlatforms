#include "damagecomponent.h"
#include "collisionmsg.h"
#include "healthcomponent.h"
#include "entity.h"
#include "gamestate.h"

namespace game {

DamageComponent::DamageComponent(int points, EntityGroup target) :
  mDamagePoints(points),
  mTarget(target)
{

}

} // namespace game
