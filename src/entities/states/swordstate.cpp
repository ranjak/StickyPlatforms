#include "swordstate.h"
#include "gamestate.h"
#include "rectangle.h"
#include "color.h"
#include "rect.h"
#include "hero.h"
#include <cmath>

namespace game {


SwordState::SwordState(Hero& hero) :
  HeroState(hero),
  mEndTimestamp(0),
  mDamageEndTimestamp(0),
  mSword(&hero)
{

}

void SwordState::update(uint32_t step, GameState &game)
{
  if (game.now() >= mEndTimestamp) {
    mHero.stopSword();
    return;
  }

  // Perform the slash
  if (game.now() < mDamageEndTimestamp)
    mSword.update(step, game);
}

void SwordState::enter()
{
  // The state lasts 300ms
  mEndTimestamp = GameState::current().now() + 300;
  // The swing must last 50ms, start above the hero and end at his feet.
  mDamageEndTimestamp = GameState::current().now() + 50;

  // Reset the sword's position
  const Rect<float> &box = mHero.getLocalBox();
  mSword.setLocalPos(Vector<float>(box.w, - mSword.getLocalBox().h));
}

void SwordState::draw(Display &display, const Camera &camera) const
{
  mSword.draw(display, camera);
}

} // namespace game
