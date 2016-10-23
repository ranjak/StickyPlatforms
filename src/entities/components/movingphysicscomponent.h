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
  static const float GRAVITY;
  static const float FALL_SPEED;

  MovingPhysicsComponent(Entity &owner, bool isObstacle=true, bool hasGravity=true);

  ~MovingPhysicsComponent();

  void update(std::uint32_t step, GameState &game) override;

  Vector<float> &velocity() { return mVelocity; }

  /**
   * @brief addAcceleration Apply an acceleration on the entity.
   * The total acceleration applied to the entity is reset to 0 after every update.
   * @param acceleration Acceleration vector in pixels/s²
   */
  void addAcceleration(const Vector<float> &acceleration);

  bool isOnGround();

  void setGravityEnabled(bool enabled);

  bool ignoresObstacles() { return mIgnoresObstacles; }
  void setIgnoresObstacles(bool ignore);

  void collide(PhysicsComponent &other);
  void collide(Tile &tile, const Vector<int> &location);

private:
  void collisionResponse(const Vector<int> &normal);

private:
  Vector<float> mVelocity;
  Vector<float> mRemainder;
  Vector<float> mAccelerationSum;
  bool mIsOnGround;
  bool mHasGravity;
  bool mIgnoresObstacles;

  // Pair: tile position => collision normal
  std::vector<std::pair<Vector<int>, Vector<int>>> mCollidingTiles;
};

} // namespace game

#endif // MOVINGPHYSICSCOMPONENT_H
