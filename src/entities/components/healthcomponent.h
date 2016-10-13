#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "component.h"
#include <cstdint>

namespace game {

class Entity;
struct Message;

class HealthComponent : public Component
{
public:
  HealthComponent(Entity &owner, int hp);

  void receiveMessage(Message &msg);

private:
  Entity &mEntity;
  int mHealthPoints;
  std::uint32_t mInvincibilityEnd;
};

} // namespace game

#endif // HEALTHCOMPONENT_H
