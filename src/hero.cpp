#include "hero.h"

namespace game {


Hero::Hero() :
  Entity(),
  velocity(0.f, 0.f),
  cube(0, 0, 20, 20)
{

}

void Hero::update(uint32_t step, GameState &game)
{
  // Set the velocity according to user input
  velocity.x = 0.f;
  velocity.y = 0.f;

  if (game.isCommandPressed(Command::LEFT))
    velocity.x -= 100.f;
  if (game.isCommandPressed(Command::RIGHT))
    velocity.x += 100.f;
  if (game.isCommandPressed(Command::DOWN))
    velocity.y += 100.f;
  if (game.isCommandPressed(Command::UP))
    velocity.y -= 100.f;

  // Move the hero according to its velocity
  boundingBox.x += (int) (velocity.x * step / 1000.f);
  boundingBox.y += (int) (velocity.y * step / 1000.f);

  // Update the graphics
  // TODO remove duplicate data ?
  cube.setPos(boundingBox.x, boundingBox.y);
}

void Hero::draw(Display& target) const
{
  cube.draw(target);
}

} // namespace game
