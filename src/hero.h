#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "rectangle.h"
#include "herostate.h"
#include "gamevector.h"
#include "swordstate.h"
#include <memory>

namespace game {

class HeroState;
class Camera;

class Hero : public Entity
{
public:
  Hero();

  void update(uint32_t step, GameState &game) override;

  void onObstacleReached(const Vector<int> &normal) override;

  void draw(Display &target, const Camera &camera) const override;

  Vector<float> &velocity();

  void swingSword();
  void stopSword();

private:
  void updatePhysics(uint32_t step, GameState &game);

private:
  // State the hero is currently in
  std::unique_ptr<HeroState> mState;
  // Velocity in pixels/sec
  Vector<float> mVelocity;
  bool mOnGround;
  // Fractional movement
  Vector<float> mRemainder;
  // Sword-realted stuff
  bool mIsSlashing;
  SwordState mSwordState;
};

} //namespace game

#endif // HERO_H
