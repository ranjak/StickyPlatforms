#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "component.h"
#include "gamevector.h"
#include "entity.h"
#include <cstdint>
#include <vector>

namespace game {

class GameState;
class MovementComponent;
class Tile;
template<typename T> class Rect;

class PhysicsComponent : public Component
{
public:
  PhysicsComponent(Entity &owner);

  ~PhysicsComponent();

  void update(std::uint32_t step, GameState &game) override;

  Vector<float> &velocity() { return mVelocity; }

  bool isOnGround();

  bool isObstacle() { return mIsObstacle; }

  Entity &entity() { return mEntity; }

  void collide(PhysicsComponent &other);
  void collide(Tile &tile, const Vector<int> &location);

private:
  void collisionResponse(const Vector<int> &normal);

private:
  Vector<float> mVelocity;
  Vector<float> mRemainder;
  float mGravity;
  float mFallSpeed;
  bool mIsObstacle;
  bool mIsOnGround;
  Entity& mEntity;

  std::vector<EntityID> mCollidingEntities;
  std::vector<Vector<int>> mCollidingTiles;
};

} // namespace game

#endif // PHYSICSCOMPONENT_H
