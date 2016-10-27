#include "damagecomponent.h"
#include "collision.h"
#include "healthcomponent.h"
#include "entity.h"
#include "gamestate.h"

namespace game {

DamageComponent::DamageComponent(int points, EntityGroup target, bool ignoreInvincibility) :
  direction(0),
  mDamagePoints(points),
  mTarget(target),
  mIgnoreInvincibility(ignoreInvincibility)
{

}

} // namespace game
