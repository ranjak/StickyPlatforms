#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "rectangle.h"
#include "physicsobject.h"

namespace game {


class Hero : public Entity
{
public:
  Hero();

  void update(uint32_t step, GameState &game) override;

  void draw(Display& target) const override;

private:
  // Physics component for our hero
  PhysicsObject physics;
  // Visual representation (aka white cube)
  Rectangle cube;
};

} //namespace game

#endif // HERO_H
