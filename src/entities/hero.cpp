#include "hero.h"
#include "airstate.h"
#include "groundstate.h"
#include "world/tile.h"
#include "camera.h"
#include "rectangle.h"
#include "gamestate.h"
#include "playerinputcomponent.h"
#include <algorithm>
#include <cmath>
#include <memory>

namespace game {


Hero::Hero() :
  Entity(0, 0, Tile::SIZE, Tile::SIZE, true, "Hero", std::unique_ptr<Graphics>(new Rectangle(Tile::SIZE, Tile::SIZE, Color::GREEN)))
//  mIsSlashing(false),
//  mSwordState(*this)
{
  std::unique_ptr<MovementComponent> mvt(new MovementComponent(*this));
  std::unique_ptr<Component> physics(new PhysicsComponent(*mvt));
  std::unique_ptr<WalkComponent> walk(new WalkComponent(*mvt));

  addComponent(std::unique_ptr<Component>(new PlayerInputComponent(*this, *walk)));
  addComponent(std::move(walk));
  addComponent(std::move(physics));
  addComponent(std::move(mvt));
}

void Hero::draw(Display &target, const Camera &camera) const
{
  Entity::draw(target, camera);

//  if (mIsSlashing)
//    mSwordState.draw(target, camera);
}

void Hero::swingSword()
{
//  // We can't start a new slash before the current one is done.
//  if (mIsSlashing)
//    return;

//  mIsSlashing = true;
//  mSwordState.enter();
}

void Hero::stopSword()
{
//  mIsSlashing = false;
}

} // namespace game
