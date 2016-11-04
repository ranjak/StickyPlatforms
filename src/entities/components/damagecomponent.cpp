#include "damagecomponent.h"
#include "collision.h"
#include "healthcomponent.h"
#include "entity.h"
#include "game.h"

namespace game {

DamageComponent::DamageComponent(int points, EntityGroup target, float knockback, bool ignoreInvincibility) :
  direction(0),
  knockback(knockback),
  mDamagePoints(points),
  mTarget(target),
  mIgnoreInvincibility(ignoreInvincibility)
{

}

} // namespace game
