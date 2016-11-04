#ifndef LEVEL_H
#define LEVEL_H

#include "gamevector.h"
#include "world/tile.h"
#include "world/tileset.h"
#include "entitymanager.h"
#include "entity.h"
#include <vector>
#include <memory>
#include <string>

namespace game {

class Game;
class Tile;
class Display;
template<typename T> class Rect;
class MovingPhysicsComponent;

/**
 * @brief The Level class represents a game level.
 * A level is made of tiles, and contains game entities.
 */
class Level
{
public:

  Level(int width, int height, TilesetList &&tilesets, std::unique_ptr<TileID[]> tiles);

  static std::unique_ptr<Level> loadFromTmx(const std::string &file, Display &display);

  void update(Game &game, std::uint32_t step);

  void draw(Display &target, const Game &game);

  TileID *tiles();

  void checkTileCollisions(MovingPhysicsComponent &object);

  bool start(const std::string &startingPoint="playerStart");

  EntityManager &entities();
  const EntityManager &entities() const;

  /**
   * @brief getObstaclesInArea Get the bounding boxes of every tile that is an obstacle for \p object in a given area.
   * @param area Rectangle in world pixel coordinates.
   * @param object Physics component of the entity relative to which the check is performed.
   * @return Bounding boxes of every tile in \p area.
   */
  std::vector<Rect<float>> getTileObstaclesInArea(Rect<float> area, const MovingPhysicsComponent &object) const;

  Tile *getTileAt(const Vector<int> &tilePos);
  const Tile *getTileAt(const Vector<int> &tilePos) const;

  /**
   * @brief getFacingObstacle Cehck whether an entity is facing a blocking tile along the given direction.
   * @param box Bounding box of the entity
   * @param direction X or Y direction (one axis at a time)
   * @param[out] obstacle Coordinates of the closest tile obstacle, if any
   * @param maxPoint Maximum distance in pixels along the direction to check for obstacles.
   * If unspecified, the maximum distance is the facing level's boundary.
   * @return \c true if an obstacle was found.
   */
  bool getFacingObstacle(const Rect<float> &box, const Vector<float> &direction, Vector<int> &obstacle, int maxPoint);
  bool getFacingObstacle(const Rect<float> &box, const Vector<float> &direction, Vector<int> &obstacle);

  Entity *getHero();

  const Vector<int> &getSize() const;
  Vector<float> getPixelSize() const;

  const std::string &getFilename() const;

private:
  std::string mFilename;
  // Size in tiles
  Vector<int> mSize;
  TilesetList mTilesets;
  // Static tiles the world is made of
  std::unique_ptr<TileID[]> mTiles;
  // Dynamic entities
  EntityManager mEntities;
  EntityID mHeroId;
};

}


#endif // LEVEL_H
