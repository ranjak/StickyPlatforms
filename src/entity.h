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

class Entity
{
public:
  Entity();
  Entity(int x, int y, int w=0, int h=0, std::unique_ptr<Graphics> graphs=nullptr);

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

  Vector<float> getPosition() const;

  void setPosition(const Vector<float>& newPos);

  Rect<float>& getBoundingBox();
  const Rect<float>& getBoundingBox() const;

protected:
  Rect<float> mBoundingBox;
  std::unique_ptr<Graphics> mGraphics;
};

} // namespace game

#endif // ENTITY_H
