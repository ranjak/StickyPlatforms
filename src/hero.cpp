#include "hero.h"
#include <algorithm>

namespace game {


float Hero::GRAVITY = 100.f;

Hero::Hero() :
  Entity(0, 0, 20, 20),
  fallSpeed(200.f),
  state(State::AIR),
  velocity(0.f, 0.f),
  cube(0, 0, 20, 20)
{

}

void Hero::update(uint32_t step, GameState &game)
{
  velocity.x = 0.f;
  // These inputs work for both states
  if (game.isCommandPressed(Command::LEFT))
    velocity.x -= 100.f;
  if (game.isCommandPressed(Command::RIGHT))
    velocity.x += 100.f;

  boundingBox.x += (int) (velocity.x * step / 1000.f);

  switch (state) {

  case State::GROUND:
    // Jump: set initial jump speed
    if (game.isCommandPressed((Command::JUMP)))
      velocity.y = - fallSpeed * 2;

    boundingBox.y += (int) (velocity.y * step / 1000.f);
    break;

  case State::AIR:
    // Apply gravity (accelerate until max fall speed)
    velocity.y = std::min(fallSpeed, velocity.y + (GRAVITY * step / 1000.f));

    boundingBox.y += (int) (velocity.y * step / 1000.f);
    break;
  }

  // Collision handling
  // If we find out we're not currently on the ground, we'll switch to air state
  // First check the base ground level. FIXME: do not hardcode
  bool onGround = boundingBox.y + boundingBox.h >= 240;
  if (onGround) {
    boundingBox.y = 240 - boundingBox.h;
    velocity.y = 0;
  }
  // Check collisions
  for (CollisionManifold& col : game.checkCollisions(*this)) {

    // Nullify velocity if it's opposite to the normal
    if (velocity.x * col.normal.x < 0)
      velocity.x = 0;
    else if (velocity.y * col.normal.y < 0)
      velocity.y = 0;

    // Collision from above (= on ground)
    if (col.normal.y == -1) {
      onGround = true;
      // Place the bounding box correctly (we don't want them to overlap)
      boundingBox.y = col.collider.getBoundingBox().y - boundingBox.h;
    }
    // From below
    else if (col.normal.y == 1)
      boundingBox.y = col.collider.getBoundingBox().y + col.collider.getBoundingBox().h;

    // From the side
    else if (col.normal.x == -1)
      boundingBox.x = col.collider.getBoundingBox().x - boundingBox.w;

    else if (col.normal.x == 1)
      boundingBox.x = col.collider.getBoundingBox().x + col.collider.getBoundingBox().w;
  }

  if (!onGround)
    state = State::AIR;
  else
    state = State::GROUND;

  // Update the graphics
  // TODO remove duplicate data ?
  cube.setPos(boundingBox.x, boundingBox.y);
}

void Hero::draw(Display& target) const
{
  cube.draw(target);
}

} // namespace game
