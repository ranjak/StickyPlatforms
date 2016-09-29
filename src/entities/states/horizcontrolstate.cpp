#include "horizcontrolstate.h"
#include "gamecommands.h"
#include "gamevector.h"
#include "gamestate.h"
#include "hero.h"
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
  if (game.getCommands().isHeld(Command::LEFT) && !game.getCommands().isHeld(Command::RIGHT))
    velocity.x = std::max(-getMaxSpeed(), velocity.x - accelAmount);

  else if (game.getCommands().isHeld(Command::RIGHT) && !game.getCommands().isHeld(Command::LEFT))
    velocity.x = std::min(getMaxSpeed(), velocity.x + accelAmount);

  // No direction : decelerate until stop
  else if (velocity.x > 0)
    velocity.x = std::max(0.f, velocity.x - accelAmount);
  else if (velocity.x < 0)
    velocity.x = std::min(0.f, velocity.x + accelAmount);

  if (game.getCommands().isHit(Command::SWORD))
    mHero.swingSword();
}

float HorizControlState::getMaxSpeed()
{
  return 400.f;
}

}
