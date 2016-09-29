#include "groundstate.h"
#include "gamecommands.h"
#include "gamestate.h"
#include "hero.h"
#include "airstate.h"

namespace game {


GroundState::GroundState(Hero &hero) :
  HorizControlState(hero)
{

}

void GroundState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  // Jump: set initial jump speed
  if (game.getCommands().isHit(Command::JUMP))
    mHero.velocity().y = - 1000.f;

  if (!game.getLevel().isOnGround(mHero))
    mHero.setState(std::unique_ptr<AirState>(new AirState(mHero)));
}

float GroundState::getAcceleration()
{
  return 2500.f;
}

}
