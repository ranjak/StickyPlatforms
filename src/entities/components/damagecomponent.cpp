#include "damagecomponent.h"
#include "collision.h"
#include "healthcomponent.h"
#include "entity.h"
#include "gamestate.h"

namespace game {

DamageComponent::DamageComponent(int points, EntityGroup target, float knockback, bool ignoreInvincibility) :
  direction(0),
  mDamagePoints(points),
  knockback(knockback),
  mTarget(target),
  mIgnoreInvincibility(ignoreInvincibility)
{

}

} // namespace game
