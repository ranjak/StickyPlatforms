#include "horizcontrolstate.h"
#include <cmath>
#include <algorithm>

namespace game {


HorizControlState::HorizControlState(Hero &hero) :
  HeroState(hero)
{

}

void HorizControlState::update(std::uint32_t step, GameState &game)
{
  Vector<float>& velocity = mHero.velocity();

  float accelAmount = getAcceleration() * step / 1000.f;

  // Accelerate until max speed
  if (game.isCommandPressed(Command::LEFT) && !game.isCommandPressed(Command::RIGHT))
    velocity.x = std::max(-getMaxSpeed(), velocity.x - accelAmount);

  else if (game.isCommandPressed(Command::RIGHT) && !game.isCommandPressed(Command::LEFT))
    velocity.x = std::min(getMaxSpeed(), velocity.x + accelAmount);

  // No direction : decelerate until stop
  else if (velocity.x > 0)
    velocity.x = std::max(0.f, velocity.x - accelAmount);
  else if (velocity.x < 0)
    velocity.x = std::min(0.f, velocity.x + accelAmount);
}

float HorizControlState::getMaxSpeed()
{
  return 300.f;
}

}
