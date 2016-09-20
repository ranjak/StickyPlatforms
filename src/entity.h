#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <memory>
#include "gamevector.h"
#include "rect.h"
#include "graphics.h"

namespace game {

class GameState;
class Display;
class Camera;
class Graphics;
class Tile;

class Entity
{
public:
  Entity();
  Entity(int x, int y, int w=0, int h=0, std::unique_ptr<Graphics> graphs=nullptr, Entity *parent=nullptr, bool isObstacle=false);

  virtual ~Entity() = default;

  /**
   * @brief update Update this entity, making its simulation advance by \p step.
   * @param step Time in milliseconds to simulate.
   * @param game Currently running game.
   */
  virtual void update(std::uint32_t step, GameState& game);

  /**
   * @brief draw Draw this entity on \p target, if applicable.
   * @param target Render target on which the entity will be drawn.
   */
  virtual void draw(Display& target, const Camera& camera) const;

  /**
   * @brief onObstacleReached Function called by the level when this entity's movement
   * is stopped by an obstacle (a tile or another entity).
   * @param normal Normal of the collision.
   * The normal is the direction of the obstacle's face on which the collision occured.
   */
  virtual void onObstacleReached(const Vector<int> &normal);

  virtual void onCollision(Entity &entity);
  virtual void onCollision(Tile &tile, Vector<int> pos);

  /** Whether this entity can block the way of other entities */
  bool isObstacle() const;
  /** Whether this entity should be checked for collisions */
  bool isCollidable() const;
  bool ignoresObstacles() const;

  virtual bool isDead() const;

  Vector<float> getLocalPos() const;
  Vector<float> getGlobalPos() const;

  void setLocalPos(const Vector<float>& newPos);
  void setGlobalPos(const Vector<float> &newPos);

  Rect<float> &getLocalBox();
  const Rect<float> &getLocalBox() const;
  Rect<float> getGlobalBox() const;

protected:
  Rect<float> mBoundingBox;
  std::unique_ptr<Graphics> mGraphics;
  Entity *mParent;
  bool mIsObstacle;
  bool mIsCollidable;
  bool mIgnoresObstacles;
};

} // namespace game

#endif // ENTITY_H
