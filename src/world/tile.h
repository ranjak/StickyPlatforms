#ifndef TILE_H
#define TILE_H

#include <memory>

namespace game {


class Graphics;
class Display;
class Camera;

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

  Tile(bool isObstacle, const std::shared_ptr<Graphics> &graphics = std::shared_ptr<Graphics>());

  bool isObstacle() const;
  void draw(Display& display, int x, int y, const Camera &cam) const;

private:
  bool mIsObstacle;
  std::shared_ptr<Graphics> mGraphics;
};

}

#endif // TILE_H
