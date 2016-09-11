#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include "gamestate.h"
#include "gamevector.h"
#include "display.h"
#include "rect.h"

namespace game {

class GameState;
class Display;

class Entity
{
public:
  Entity();
  Entity(int x, int y, int w=0, int h=0);

  /**
   * @brief update Update this entity, making its simulation advance by \p step.
   * @param step Time in milliseconds to simulate.
   * @param game Currently running game.
   */
  virtual void update(std::uint32_t step, GameState& game) = 0;

  /**
   * @brief draw Draw this entity on \p target, if applicable.
   * @param target Render target on which the entity will be drawn.
   */
  virtual void draw(Display& target) const = 0;

  /**
   * @brief onObstacleReached Function called by the level when this entity's movement
   * is stopped by an obstacle (a tile or another entity).
   * @param normal Normal of the collision.
   * The normal is the direction of the obstacle's face on which the collision occured.
   */
  virtual void onObstacleReached(const Vector<int> &normal);

  Vector<int> getPosition() const;

  void setPosition(const Vector<int>& newPos);

  Rect<float>& getBoundingBox();
  const Rect<float>& getBoundingBox() const;

protected:
  Rect<float> mBoundingBox;
};

} // namespace game

#endif // ENTITY_H
