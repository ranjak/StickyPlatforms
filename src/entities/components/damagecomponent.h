#ifndef DAMAGECOMPONENT_H
#define DAMAGECOMPONENT_H

#include "component.h"
#include "entitygroup.h"

namespace game {

struct Message;

class DamageComponent : public Component
{
public:
  DamageComponent(int points, EntityGroup target, bool ignoreInvincibility=false);

  int points() const { return mDamagePoints; }

  EntityGroup target() const { return mTarget; }

  bool ignoresInvincibility() const { return mIgnoreInvincibility; }

public:
  // Direction of the attack, if this Damage instance is bound to an attack (sword...)
  int direction;
private:
  int mDamagePoints;
  EntityGroup mTarget;
  bool mIgnoreInvincibility;
};

} // namespace game

#endif // DAMAGECOMPONENT_H
