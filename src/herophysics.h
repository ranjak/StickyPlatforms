#ifndef HEROPHYSICS_H
#define HEROPHYSICS_H

#include "gamestate.h"
#include "hero.h"
#include "rect.h"
#include "gamevector.h"
#include <cstdint>

namespace game {


class HeroPhysics
{
public:
  HeroPhysics(Hero &hero);

  void update(std::uint32_t step, GameState &game);

  // Getter / setter functions
  /*Vector<float>& velocity();
  int& x();
  int& y();
  int& w();
  int& h();*/

private:
  Rect<float> mBoundingBox;
  Vector<float> mVelocity;
  Hero& mHero;
};

}

#endif // HEROPHYSICS_H
