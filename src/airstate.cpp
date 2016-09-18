#include "airstate.h"
#include "gamevector.h"
#include "hero.h"

namespace game {

const float AirState::GRAVITY = 2500.f;

const float AirState::FALL_SPEED = 500.f;

AirState::AirState(Hero& hero) :
    HorizControlState(hero)
{

}

void AirState::update(std::uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  // Apply gravity (accelerate until max fall speed)
    Vector<float> &velocity = mHero.velocity();

    if (velocity.y < FALL_SPEED)
        velocity.y += GRAVITY * step / 1000.f;
    else
        velocity.y = FALL_SPEED;
}

void AirState::enter()
{
}

float AirState::getAcceleration()
{
  return 600.f;
}

}
