#include "damagecomponent.h"
#include "collision.h"
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
