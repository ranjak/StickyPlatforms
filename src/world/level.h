#ifndef LEVEL_H
#define LEVEL_H

#include "gamevector.h"
#include "world/tile.h"
#include "world/tileset.h"
#include <vector>
#include <memory>

namespace game {

class GameState;
class Entity;
class Tile;
class Hero;
class Display;
template<typename T> class Rect;

/**
 * @brief The Level class represents a game level.
 * A level is made of tiles, and contains game entities.
 */
class Level
{
public:

  Level(int width, int height, std::unique_ptr<Hero> hero, TilesetList &&tilesets, std::unique_ptr<TileID[]> tiles);

  static std::unique_ptr<Level> loadFromTmx(const std::string &file, Display &display);

  void update(GameState &game, std::uint32_t step);

  void draw(Display &target, const GameState &game) const;

  void addEntity(std::unique_ptr<Entity> entity);

  TileID *tiles();

  void handleCollisions(Entity &entity);

  /**
   * @brief tryMoving Try to move \p entity to \p dest, according to a linear trajectory.
   * Obstacles are taken into account, and can prevent \p entity from reaching \p dest.
   * In any case, \p entity 's position will be updated to reflect its actual movement.
   * @param[in,out] entity The entity we want to move
   * @param dest The destination of the movement
   * @return \c true if \p entity could reach \p dest.
   */
  bool tryMoving(Entity &entity, const Vector<float> &dest);

  /**
   * @brief getEntitiesInArea Get every entity which bounding box intersects with \p area.
   * @param area Rectangular area to scan for entities.
   * @param pred Optional predicate to filter the entities.
   * @return Vector containing pointers to the entities that overlap \p area.
   * You do not own these pointers.
   */
  template<typename Func>
  std::vector<Entity *> getEntitiesInArea(const Rect<float> &area, Func &&pred=[](){return true;});

  /**
   * @brief isOnGround Check whether \p entity is standing on ground (solid tile or entity).
   * @param entity
   * @return \c true if \p entity is on ground.
   */
  bool isOnGround(Entity &entity);

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

  Hero* getHero();

  const Vector<int> &getSize();
  Vector<int> getPixelSize();

private:
  // Size in tiles
  Vector<int> mSize;
  TilesetList mTilesets;
  // Static tiles the world is made of
  std::unique_ptr<TileID[]> mTiles;
  // Dynamic entities
  std::vector<std::unique_ptr<Entity>> mEntities;
  Hero* mHero;
};

}


#endif // LEVEL_H
