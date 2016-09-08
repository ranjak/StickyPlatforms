#include "groundstate.h"

namespace game {


GroundState::GroundState(Hero &hero) :
  HorizControlState(hero)
{

}

void GroundState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  // Jump: set initial jump speed
  if (game.isCommandPressed((Command::JUMP)))
    mHero.velocity().y = - 800.f;
}

float GroundState::getAcceleration()
{
  return 2000.f;
}

}
