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
  mSword(0, 0, 25, 8, std::unique_ptr<Graphics>(new Rectangle(25, 8, Color::BLUE))),
  mSwordVelocity(0.f),
  mSwordMvt(0)
{

}

void SwordState::update(uint32_t step, GameState &game)
{
  if (game.now() >= mEndTimestamp) {
    mHero.stopSword();
    return;
  }

  Rect<float> &hbox = mHero.getBoundingBox();
  Rect<float> &sbox = mSword.getBoundingBox();

  // Perform the slash
  if (game.now() < mDamageEndTimestamp) {
    float integerPart;
    mRemainder += mSwordVelocity * step / 1000.f;

    mRemainder = std::modf(mRemainder, &integerPart);
    mSwordMvt += integerPart;
  }

  sbox.x = hbox.x + hbox.w + 10;
  sbox.y = hbox.y - sbox.h + mSwordMvt;
}

void SwordState::enter()
{
  // The state lasts one second
  mEndTimestamp = GameState::current().now() + 300;
  mDamageEndTimestamp = GameState::current().now() + 50;

  // The swing must last 300ms, start above the hero and end at his feet.
  Rect<float> &hbox = mHero.getBoundingBox();
  Rect<float> &sbox = mSword.getBoundingBox();

  mSwordVelocity = hbox.h / (50.f / 1000.f);

  // Put the sword above the hero
  sbox.y = hbox.y - sbox.h;
  mSwordMvt = 0;
}

void SwordState::draw(Display &display, const Camera &camera) const
{
  mSword.draw(display, camera);
}

} // namespace game
