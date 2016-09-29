#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "movementcomponent.h"
#include "physicscomponent.h"

namespace game {

class Enemy : public Entity
{
public:
  Enemy(int x, int y, int w, int h, const std::string &name);

  virtual void update(std::uint32_t step, GameState& game) override;

  virtual void onObstacleReached(const Vector<int> &normal) override;
  virtual void onCollision(Entity &entity) override;

  virtual bool isDead() const override;

private:
  float mSpeed;
  MovementComponent mMovement;
  PhysicsComponent mPhysics;
  int mHealthPoints;
  std::uint32_t mInvincibilityEnd;
};

} // namespace game

#endif // ENEMY_H
