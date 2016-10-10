#ifndef LEVEL_H
#define LEVEL_H

#include "gamevector.h"
#include "world/tile.h"
#include "world/tileset.h"
#include "entitymanager.h"
#include "entity.h"
#include <vector>
#include <memory>

namespace game {

class GameState;
class Tile;
class Display;
template<typename T> class Rect;

/**
 * @brief The Level class represents a game level.
 * A level is made of tiles, and contains game entities.
 */
class Level
{
public:

  Level(int width, int height, TilesetList &&tilesets, std::unique_ptr<TileID[]> tiles);

  static std::unique_ptr<Level> loadFromTmx(const std::string &file, Display &display);

  void update(GameState &game, std::uint32_t step);

  void draw(Display &target, const GameState &game) const;

  TileID *tiles();

  void handleCollisions(Entity &entity);

  bool start(const std::string &startingPoint="playerStart");

  /**
   * @brief tryMoving Try to move \p entity to \p dest, according to a linear trajectory.
   * Obstacles are taken into account, and can prevent \p entity from reaching \p dest.
   * In any case, \p entity 's position will be updated to reflect its actual movement.
   * @param[in,out] entity The entity we want to move
   * @param dest The destination of the movement
   * @return \c true if \p entity could reach \p dest.
   */
  bool tryMoving(Entity &entity, const Vector<float> &dest);

  EntityManager &entities();

  /**
   * @brief isOnGround Check whether \p entity is standing on ground (solid tile or entity).
   * @param entity
   * @return \c true if \p entity is on ground.
   */
  bool isOnGround(Entity &entity) const;

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

  const Vector<int> &getSize();
  Vector<int> getPixelSize();

private:
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
