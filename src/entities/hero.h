#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "rectangle.h"
#include "herostate.h"
#include "gamevector.h"
#include "movementcomponent.h"
#include "physicscomponent.h"
#include "walkcomponent.h"
#include <memory>

namespace game {

class HeroState;
class Camera;

class Hero : public Entity
{
public:
  Hero();

  void draw(Display &target, const Camera &camera) const override;

  void swingSword();
  void stopSword();

private:
  // Sword-realted stuff
//  bool mIsSlashing;
//  SwordState mSwordState;
};

} //namespace game

#endif // HERO_H
