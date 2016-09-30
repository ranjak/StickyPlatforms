#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

namespace game {

class Enemy : public Entity
{
public:
  Enemy(int x, int y, int w, int h, const std::string &name);

  virtual void onCollision(Entity &entity) override;

  virtual bool isDead() const override;

private:
  float mSpeed;
  int mHealthPoints;
  std::uint32_t mInvincibilityEnd;
};

} // namespace game

#endif // ENEMY_H
