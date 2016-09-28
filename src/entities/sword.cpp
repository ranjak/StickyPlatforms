#include "sword.h"
#include "rectangle.h"

namespace game {

Sword::Sword(Entity *wielder) :
  Entity(0, 0, 25, 8, false, "Sword", std::unique_ptr<Graphics>(new Rectangle(25, 8, Color::BLUE)), wielder),
  mMovement(*this)
{
  mIgnoresObstacles = true;

  // The sword will go from the wielder's head to feet in 50 ms
  mMovement.velocity().y = wielder->getLocalBox().h / (50.f / 1000.f);
}

void Sword::update(uint32_t step, GameState &game)
{
  mMovement.update(step, game);
}

} // namespace game
