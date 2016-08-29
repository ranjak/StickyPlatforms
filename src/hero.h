#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "rectangle.h"

namespace game {


class Hero : public Entity
{
public:
  Hero();

  void update(uint32_t step, GameState &game) override;

  void draw(Display& target) const override;

private:
  // Velocity in pixels/sec
  Vector<float> velocity;
  // Visual representation (aka white cube)
  Rectangle cube;
};

} //namespace game

#endif // HERO_H
