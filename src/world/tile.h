#ifndef TILE_H
#define TILE_H

#include <memory>
#include <functional>
#include "rect.h"
#include "gamevector.h"

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
  constexpr static int SIZE = 32;

  constexpr static Vector<int> pixelToTile(float x, float y)
  {
    return Vector<int>(static_cast<int>(x) / Tile::SIZE, static_cast<int>(y) / Tile::SIZE);
  }

  Tile(TileID id,
       bool isObstacle = true,
       const std::shared_ptr<Image> &graphics = std::shared_ptr<Image>(),
       const Rect<int> &graphicsPos = {},
       const Rect<float> &collisionBox = { 0.f, 0.f, static_cast<float>(SIZE), static_cast<float>(SIZE) },
       const std::function<void(Entity &, Level &)> &collisionBehavior = std::function<void(Entity &, Level &)>()
      );

  /**
   * @brief isObstacle Whether this tile is an obstacle for moving entities.
   * @return \c true if entities can't traverse this tile.
   */
  bool isObstacle() const;

  /**
   * @brief getCollisionBox Get the rectangle representing the collision area for this tile.
   * @param x X position of the tile in tile coordinates
   * @param y Y position of the tile in tile coordinates
   * @return The collision box for this tile, offset by the proper coordinates in pixels.
   */
  Rect<float> getCollisionBox(int x, int y) const;
  Rect<float> getCollisionBox(const Vector<int> &pos) const { return getCollisionBox(pos.x, pos.y); }

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
  Rect<float> mCollisionBox;
};

}

#endif // TILE_H
