#include "airstate.h"
#include "gamevector.h"
#include "hero.h"
#include "groundstate.h"
#include "gamestate.h"

namespace game {

AirState::AirState(Hero& hero) :
    HorizControlState(hero)
{

}

void AirState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  if (game.getLevel().isOnGround(mHero))
    mHero.setState(std::unique_ptr<GroundState>(new GroundState(mHero)));
}

float AirState::getAcceleration()
{
  return 1200.f;
}

}
