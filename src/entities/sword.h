#ifndef SWORD_H
#define SWORD_H

#include "entity.h"
#include "movementcomponent.h"

namespace game {

class Sword : public Entity
{
public:
  Sword(Entity *wielder);

  void update(std::uint32_t step, GameState &game) override;

private:
  MovementComponent mMovement;
};

} // namespace game

#endif // SWORD_H
