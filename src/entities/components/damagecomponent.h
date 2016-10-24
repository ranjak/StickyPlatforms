#ifndef DAMAGECOMPONENT_H
#define DAMAGECOMPONENT_H

#include "component.h"
#include "entitygroup.h"

namespace game {

struct Message;

class DamageComponent : public Component
{
public:
  DamageComponent(int points, EntityGroup target);

  int points() const { return mDamagePoints; }

  EntityGroup target() const { return mTarget; }

private:
  int mDamagePoints;
  EntityGroup mTarget;
};

} // namespace game

#endif // DAMAGECOMPONENT_H
