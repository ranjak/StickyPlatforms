#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "rectangle.h"
#include "herostate.h"
#include "gamevector.h"
#include <memory>

namespace game {

class HeroState;
class Camera;

class Hero : public Entity
{
public:
  Hero();

  void update(uint32_t step, GameState &game) override;

  void draw(Display& target, const Camera &cam) const override;

  void onObstacleReached(const Vector<int> &normal) override;

  Vector<float> &velocity();

private:
  void updatePhysics(uint32_t step, GameState &game);

private:
  // State the hero is currently in
  std::unique_ptr<HeroState> mState;
  // Velocity in pixels/sec
  Vector<float> mVelocity;
  bool mOnGround;
  // Visual representation (aka green cube)
  Rectangle mCube;
  // Fractional movement
  Vector<float> mRemainder;
};

} //namespace game

#endif // HERO_H
