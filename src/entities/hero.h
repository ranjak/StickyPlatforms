#ifndef HERO_H
#define HERO_H

#include "entity.h"

namespace game {

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
