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

  Vector<float> &velocity() { return mVelocity; }

  /**
   * @brief addAcceleration Apply an acceleration on the entity.
   * The total acceleration applied to the entity is reset to 0 after every update.
   * @param acceleration Acceleration vector in pixels/s²
   */
  void addAcceleration(const Vector<float> &acceleration);

  bool isOnGround();

  void setGravityEnabled(bool enabled);

  bool ignoresObstacles() const { return mIgnoresObstacles; }
  void setIgnoresObstacles(bool ignore);

  /**
   * @brief isIgnored Check whether this entity ignores collisions with \p other.
   * @param other ID of the entity to be tested.
   * @return \c true if collisions with \p other are ignored by \c this.
   */
  bool isIgnored(EntityID other) const;
  /**
   * @brief isIgnored Check whether this entity ignores collisions with a specific tile.
   * @param tile Position of the tile (in tile coordinates).
   * @return \c true if collisions with \p tile are ignored by \c this.
   */
  bool isIgnored(const Vector<int> &tile) const;

  /**
   * @brief setIgnored Set whether \c this should ignore collisions with \p other.
   * @param other ID of the entity to set as ignored by \c this.
   * @param ignored Whether collisions with \p other should be ignored by \c this.
   */
  void setIgnored(EntityID other, bool ignored);
  /**
   * @brief setIgnored Set whether \c this should ignore collisions with a specific tile.
   * @param tile Position of the tile in tile coordinates to set as ignored by \c this.
   * @param ignored Whether collisions with \p tile should be ignored by \c this.
   */
  void setIgnored(const Vector<int> &tile, bool ignored);

  void collide(PhysicsComponent &other);
  void collide(Tile &tile, const Vector<int> &location);

private:
  void updateDelegate(std::uint32_t step, GameState &game) override;

  void collisionResponse(const Vector<int> &normal);

private:
  Vector<float> mVelocity;
  Vector<float> mRemainder;
  Vector<float> mAccelerationSum;
  bool mIsOnGround;
  bool mHasGravity;
  bool mIgnoresObstacles;

  // Specific objects that are excluded of collision detection
  std::vector<EntityID> mIgnoredEntities;
  std::vector<Vector<int>> mIgnoredTiles;
};

} // namespace game

#endif // MOVINGPHYSICSCOMPONENT_H
