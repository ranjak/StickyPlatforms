#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "movement.h"

namespace game {

class Enemy : public Entity
{
public:
  Enemy(int x, int y, int w, int h);

  virtual void update(std::uint32_t step, GameState& game) override;
  virtual void onObstacleReached(const Vector<int> &normal) override;

private:
  float mSpeed;
  Movement mMovement;
};

} // namespace game

#endif // ENEMY_H
