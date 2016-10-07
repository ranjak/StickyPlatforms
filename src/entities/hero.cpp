#include "hero.h"
#include "world/tile.h"
#include "camera.h"
#include "rectangle.h"
#include "gamestate.h"
#include "playerinputcomponent.h"
#include "physicscomponent.h"
#include "actorcontrolcomponent.h"
#include "make_unique.h"
#include <algorithm>
#include <cmath>
#include <memory>

namespace game {


Hero::Hero() :
  Entity(0, 0, Tile::SIZE, Tile::SIZE, true, "Hero", std::unique_ptr<Graphics>(new Rectangle(Tile::SIZE, Tile::SIZE, Color::GREEN)))
//  mIsSlashing(false),
//  mSwordState(*this)
{
  std::unique_ptr<InputComponent> input = std::make_unique<PlayerInputComponent>();
  std::unique_ptr<PhysicsComponent> physics = std::make_unique<PhysicsComponent>(*this);
  std::unique_ptr<Component> control = std::make_unique<ActorControlComponent>(*this, *physics, *input, 300.f);

  addComponent(std::move(input));
  addComponent(std::move(control));
  addComponent(std::move(physics));
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
