#ifndef TILE_H
#define TILE_H

#include <memory>
#include <functional>
#include "rect.h"

namespace game {


class Image;
class Display;
class Camera;
class Entity;
class Level;

typedef int TileID;

/**
 * @brief A tile is a static "piece of world" of fixed size.
 * A game level is basically a grid of tiles.
 */
class Tile
{
public:
  /** Size of a tile in pixels. Tiles have identical width and height. */
  static const int SIZE;

  Tile(TileID id,
       bool isObstacle = true,
       const std::shared_ptr<Image> &graphics = std::shared_ptr<Image>(),
       const Rect<int> &graphicsPos = {},
       const std::function<void(Entity &, Level &)> &collisionBehavior = std::function<void(Entity &, Level &)>());

  /**
   * @brief isObstacle Whether this tile is an obstacle for moving entities.
   * @return \c true if entities can't traverse this tile.
   */
  bool isObstacle() const;

  /**
   * @brief draw Draw this tile at the given position in tile coordinates.
   * @param display
   * @param x X level coordinate in tiles (not pixels).
   * @param y
   * @param cam Current camera.
   */
  void draw(Display& display, int x, int y, const Camera &cam) const;

  /**
   * @brief onCollision Called by the level object when an entity collides with an instance of this tile.
   * @param entity Entity that collides with the tile.
   * @param level Current level.
   */
  void onCollision(Entity &entity, Level &level);

public:
  const TileID id;
private:
  bool mIsObstacle;
  std::shared_ptr<Image> mGraphics;
  std::function<void(Entity &, Level &)> mCollisionBehavior;
  Rect<int> mGraphicsPos;
};

}

#endif // TILE_H
