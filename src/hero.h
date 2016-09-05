#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "rectangle.h"

namespace game {


class Hero : public Entity
{
public:
  // Gravity: acceleration in pixels/s²
  static float GRAVITY;
  Hero();

  void update(uint32_t step, GameState &game) override;

  void draw(Display& target) const override;

private:
  // Maximum falling speed in pixels/s
  float fallSpeed;
  // State the hero is in at a given moment
  enum class State { GROUND, AIR };
  State state;
  // Velocity in pixels/sec
  Vector<float> velocity;
  // Visual representation (aka white cube)
  Rectangle cube;
};

} //namespace game

#endif // HERO_H
