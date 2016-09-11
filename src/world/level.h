#ifndef LEVEL_H
#define LEVEL_H

#include "gamestate.h"
#include "display.h"
#include "entity.h"
#include "gamevector.h"
#include "world/tile.h"
#include "rect.h"
#include <vector>
#include <memory>

namespace game {

struct CollisionManifold;
class Entity;
class Tile;

/**
 * @brief The Level class represents a game level.
 * A level is made of tiles, and contains game entities.
 */
class Level
{
public:
  static Tile tileset[];

  Level(int width=0, int height=0);

  void update(GameState &game, std::uint32_t step);

  void draw(Display &target) const;

  void addEntity(std::unique_ptr<Entity> entity);

  const std::unique_ptr<TileID[]>& tiles();

  /**
   * @brief collides Checks whether an entity is colliding with any other in the game world.
   * @param entity Entity we want to check collisions for.
   * @return true if entity collides with one or more other entities.
   */
  bool collides(const Entity& entity);

  /**
   * @brief checkCollisions Get a list of collisions for a given entity
   * @param entity Entity we want to check collisions for.
   * @return A list of collisions manifolds for every entity that the given entity collides with.
   */
  std::vector<CollisionManifold> checkCollisions(Entity& entity);

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
   * @return Vector containing pointers to the entities that overlap \p area.
   * You do not own these pointers.
   */
  std::vector<Entity *> getEntitiesInArea(const Rect<float> &area);

  bool isOnGround(Entity &entity);

private:
  // Size in tiles
  Vector<int> mSize;
  // Static tiles the world is made of
  std::unique_ptr<TileID[]> mTiles;
  // Dynamic entities
  std::vector<std::unique_ptr<Entity>> mEntities;
};

struct CollisionManifold {
  Entity& collider;
  Vector<int> normal;
};

}


#endif // LEVEL_H
