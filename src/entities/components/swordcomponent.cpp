#include "swordcomponent.h"
#include "movingphysicscomponent.h"
#include "entity.h"
#include "gamestate.h"
#include "rect.h"
#include "gamevector.h"
#include "damagecomponent.h"
#include <cassert>

namespace game {

SwordComponent::SwordComponent(MovingPhysicsComponent &physics) :
  mPhysics(physics),
  mDamageEnd(0),
  mSwingEnd(0)
{
  physics.entity().setEnabled(false);
}

void SwordComponent::update(uint32_t step, GameState &game)
{
  if (game.now() >= mSwingEnd)
    mPhysics.entity().setEnabled(false);

  else if (game.now() >= mDamageEnd) {
    mPhysics.velocity().y = 0.f;
    mPhysics.setCollidable(false);
  }
}

void SwordComponent::swing(int direction)
{
  std::uint32_t now = GameState::current().now();

  // Start swinging only if we aren't currently performing a swing
  if (now > mSwingEnd) {
    mSwingEnd = now + 300;
    mDamageEnd = now + 50;

    // Place the sword above the wielder's head in the direction it's facing
    Entity &sword = mPhysics.entity();
    Entity *wielder = sword.getParent();
    assert(wielder);

    sword.getComponent<DamageComponent>()->direction = direction;

    const Rect<float> &wBox = wielder->getLocalBox();

    float posX = (direction < 0) ? -sword.getLocalBox().w : wBox.w;

    sword.setLocalPos(Vector<float>(posX, -sword.getLocalBox().h));

    // Set the swing speed
    mPhysics.velocity().y = wBox.h / (50.f / 1000.f);

    sword.setEnabled(true);
    mPhysics.setCollidable(true);
  }
}

Entity &SwordComponent::getEntity()
{
  return mPhysics.entity();
}

} // namespace game
