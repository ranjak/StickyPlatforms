#ifndef MOVINGPHYSICSCOMPONENT_H
#define MOVINGPHYSICSCOMPONENT_H

#include "component.h"
#include "gamevector.h"
#include "entity.h"
#include "physicscomponent.h"
#include <cstdint>
#include <vector>

namespace game {

class GameState;
class Tile;
template<typename T> class Rect;

class MovingPhysicsComponent : public PhysicsComponent
{
public:
  MovingPhysicsComponent(Entity &owner, bool isObstacle=true);

  ~MovingPhysicsComponent();

  void update(std::uint32_t step, GameState &game) override;

  Vector<float> &velocity() { return mVelocity; }

  bool isOnGround();

  const std::vector<std::pair<Vector<int>, Vector<int>>> &getCollidingTiles() const;

  void collide(PhysicsComponent &other);
  void collide(Tile &tile, const Vector<int> &location);

private:
  void collisionResponse(const Vector<int> &normal);

private:
  Vector<float> mVelocity;
  Vector<float> mRemainder;
  bool mIsOnGround;

  // Pair: tile position => collision normal
  std::vector<std::pair<Vector<int>, Vector<int>>> mCollidingTiles;
};

} // namespace game

#endif // MOVINGPHYSICSCOMPONENT_H
