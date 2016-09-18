#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

namespace game {

class Enemy : public Entity
{
public:
  Enemy(int x, int y, int w, int h);

  virtual void update(std::uint32_t step, GameState& game) override;
  virtual void onObstacleReached(const Vector<int> &normal) override;

private:
  int mDirectionX;
  float mSpeed;
  float mRemainder;
};

} // namespace game

#endif // ENEMY_H
