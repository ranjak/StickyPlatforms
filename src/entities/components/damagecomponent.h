#ifndef DAMAGECOMPONENT_H
#define DAMAGECOMPONENT_H

#include "component.h"
#include "entitygroup.h"

namespace game {

struct Message;

class DamageComponent : public Component
{
public:
  /**
   * @brief DamageComponent
   * @param points How many HP the victim should lose.
   * @param target Entity group(s) that will be affected by the attack.
   * @param knockback Knockback strength coefficient, multiplied by a vector relative to the direction of the attack.
   * @param ignoreInvincibility Whether this damage will be dealt to invincible targets as well.
   */
  DamageComponent(int points, EntityGroup target, float knockback=1.0f, bool ignoreInvincibility=false);

  int points() const { return mDamagePoints; }

  EntityGroup target() const { return mTarget; }

  bool ignoresInvincibility() const { return mIgnoreInvincibility; }

public:
  // Direction of the attack, if this Damage instance is bound to an attack (sword...)
  int direction;
  float knockback;
private:
  int mDamagePoints;
  EntityGroup mTarget;
  bool mIgnoreInvincibility;
};

} // namespace game

#endif // DAMAGECOMPONENT_H
