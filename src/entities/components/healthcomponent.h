#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "component.h"
#include <cstdint>

namespace game {

class Entity;
struct Message;
class HealthBar;

class HealthComponent : public Component
{
public:
  HealthComponent(Entity &owner, int hp, int maxHp);
  HealthComponent(Entity &owner, int hp);

  int getHealthPoints() const { return mHealthPoints; }

  int getMaxHealthPoints() const { return mMaxHP; }

  void setUI(HealthBar *ui);

  void setHealthPoints(int hp);

private:
  void receiveMessageDelegate(Message &msg);

private:
  Entity &mEntity;
  int mMaxHP;
  int mHealthPoints;
  std::uint32_t mInvincibilityEnd;
  HealthBar *mUI;
};

} // namespace game

#endif // HEALTHCOMPONENT_H
