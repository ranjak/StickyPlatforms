#include "hero.h"

namespace game {


Hero::Hero() :
  Entity(0, 0, 20, 20),
  physics(50),
  cube(0, 0, 20, 20)
{

}

void Hero::update(uint32_t step, GameState &game)
{
  // Compute the traction force corresponding to the user's input
  Vector<int> inputForce(0, 0);

  if (game.isCommandPressed(Command::LEFT))
    inputForce.x -= 100;
  if (game.isCommandPressed(Command::RIGHT))
    inputForce.x += 100;
  if (game.isCommandPressed(Command::DOWN))
    inputForce.y += 100;
  if (game.isCommandPressed(Command::UP))
    inputForce.y -= 100;

  // Compute the new position according to the velocity
  physics.applyForce(inputForce);
  physics.update(step);

  boundingBox.x += (int) (physics.getVelocity().x * step / 1000.f);
  boundingBox.y += (int) (physics.getVelocity().y * step / 1000.f);

  // Update the graphics
  // TODO remove duplicate data ?
  cube.setPos(boundingBox.x, boundingBox.y);
}

void Hero::draw(Display& target) const
{
  cube.draw(target);
}

} // namespace game
