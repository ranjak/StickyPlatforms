#include "world/level.h"
#include "rectangle.h"
#include <algorithm>

namespace game {


Tile Level::tileset[] {
  Tile(false),
  Tile(true, std::shared_ptr<Graphics>(new Rectangle(Tile::SIZE, Tile::SIZE)))
};


Level::Level(int width, int height) :
  mSize(width, height),
  mTiles(),
  mEntities()
{
  if (width >= 0 && height >= 0)
    mTiles.reset(new TileID[width * height]);
}

void Level::update(GameState &game, uint32_t step)
{
  for (const std::unique_ptr<Entity>& entity : mEntities) {
    entity->update(step, game);
  }
}

void Level::draw(Display &target) const
{
  // Draw tiles. They are drawn by column.
  for (int i=0; i<mSize.x; i++) {
    for (int j=0; j<mSize.y; j++) {
      tileset[mTiles[i*mSize.y + j]].draw(target, i, j);
    }
  }

  // Draw entities
  for (const std::unique_ptr<Entity>& entity : mEntities) {
    entity->draw(target);
  }
}

void Level::addEntity(std::unique_ptr<Entity> entity)
{
  mEntities.push_back(std::move(entity));
}

const std::unique_ptr<TileID[]> &Level::tiles()
{
  return mTiles;
}

bool Level::collides(const Entity &entity)
{
  // Check only the entities after the given entity in sequential order
  auto entityPos = std::find_if(mEntities.begin(), mEntities.end(), [&] (std::unique_ptr<Entity>& vecEntity) { return vecEntity.get() == &entity; });

  for (auto it = entityPos+1; it != mEntities.end(); it++) {
    if (entity.getBoundingBox().intersects((*it)->getBoundingBox()))
      return true;
  }

  return false;
}

std::vector<CollisionManifold> Level::checkCollisions(Entity &entity)
{
  // Check only the entities after the given entity in sequential order
  auto entityPos = std::find_if(mEntities.begin(), mEntities.end(), [&] (std::unique_ptr<Entity>& vecEntity) { return vecEntity.get() == &entity; });

  std::vector<CollisionManifold> collisions;

  for (auto it = entityPos+1; it != mEntities.end(); it++) {
    if (entity.getBoundingBox().intersects((*it)->getBoundingBox()))
      collisions.push_back(CollisionManifold{**it, entity.getBoundingBox().getCollisionNormal((*it)->getBoundingBox())});
  }

  return collisions;
}

bool Level::tryMoving(Entity &entity, const Vector<float> &dest)
{
  Rect<float> &box = entity.getBoundingBox();
  Vector<float> direction(dest.x - box.x, dest.y - box.y);
  Vector<float> facingPoint;

  facingPoint.x = (direction.x > 0) ? box.x + box.w : box.x;
  facingPoint.y = (direction.y > 0) ? box.y + box.h : box.y;

  Vector<float> destFacingPoint(facingPoint.x + direction.x, facingPoint.y + direction.y);

  // Do not go beyond the level's boundaries
  if (destFacingPoint.x < 0)
    destFacingPoint.x = 0;
  else if (destFacingPoint.x > mSize.x * Tile::SIZE)
    destFacingPoint.x = mSize.x * Tile::SIZE - box.w;

  if (destFacingPoint.y < 0)
    destFacingPoint.y = 0;
  else if (destFacingPoint.y > mSize.y * Tile::SIZE)
    destFacingPoint.y = mSize.y * Tile::SIZE - box.h;

  // Store all entities that could possibly block our way
  Rect<float> movementArea;
  movementArea.x = std::min(destFacingPoint.x, box.x);
  movementArea.y = std::min(destFacingPoint.y, box.y);
  movementArea.w = (movementArea.x == box.x) ? (destFacingPoint.x - box.x) : (box.x + box.h - destFacingPoint.x);
  movementArea.h = (movementArea.y == box.y) ? (destFacingPoint.y - box.y) : (box.y + box.y - destFacingPoint.y);
  std::vector<Entity*> neighbours(getEntitiesInArea(movementArea));

  // Check obstacles on the X axis
  if (direction.x > 0) {
    // Find the closest tile obstacle on X
    for (int i=facingPoint.x / Tile::SIZE; i<=(destFacingPoint.x - 1) / Tile::SIZE; i++) {
      for (int j=box.y / Tile::SIZE; j<=(box.y + box.h - 1) / Tile::SIZE; j++) {
        if (tileset[mTiles[i*mSize.y + j]].isObstacle()) {
          destFacingPoint.x = i * Tile::SIZE;
          break;
        }
      }
    }
    // Find the closest entity obstacle on X
    for (auto it=neighbours.begin(); it != neighbours.end(); it++) {
      Rect<float> &ebox = (**it).getBoundingBox();
      if (*it != &entity && (box.y + box.h > ebox.y && box.y < ebox.y + ebox.h))
        destFacingPoint.x = std::min(destFacingPoint.x, ebox.x);
    }
    // Move on the X axis up to the closest obstacle found
    box.x = destFacingPoint.x - box.w;
    // Notify the entity if it didn't reach its goal
    if (box.x != dest.x)
      entity.onObstacleReached(Vector<int>(-1, 0));
  }
  else if (direction.x < 0) {
    for (int i=facingPoint.x / Tile::SIZE; i>=destFacingPoint.x / Tile::SIZE; i--) {
      for (int j=box.y / Tile::SIZE; j<=(box.y + box.h - 1) / Tile::SIZE; j++) {
        if (tileset[mTiles[i*mSize.y + j]].isObstacle()) {
          destFacingPoint.x = i * Tile::SIZE;
          break;
        }
      }
    }
    for (auto it=neighbours.begin(); it != neighbours.end(); it++) {
      Rect<float> &ebox = (**it).getBoundingBox();
      if (*it != &entity && (box.y + box.h > ebox.y && box.y < ebox.y + ebox.h))
        destFacingPoint.x = std::max(destFacingPoint.x, ebox.x + ebox.w);
    }
    box.x = destFacingPoint.x;
    if (box.x != dest.x)
      entity.onObstacleReached(Vector<int>(1, 0));
  }

  // Same for the Y axis
  if (direction.y > 0) {
    for (int i=box.x / Tile::SIZE; i<=(box.x + box.w - 1) / Tile::SIZE; i++) {
      for (int j=facingPoint.y / Tile::SIZE; j<=(destFacingPoint.y - 1) / Tile::SIZE; j++) {
        if (tileset[mTiles[i*mSize.y + j]].isObstacle()) {
          destFacingPoint.y = j * Tile::SIZE;
          break;
        }
      }
    }
    for (auto it=neighbours.begin(); it != neighbours.end(); it++) {
      Rect<float> &ebox = (**it).getBoundingBox();
      if (*it != &entity && (box.x + box.w > ebox.x && box.x < ebox.x + ebox.w))
        destFacingPoint.x = std::min(destFacingPoint.y, ebox.y);
    }
    box.y = destFacingPoint.y - box.h;
    if (box.y != dest.y)
      entity.onObstacleReached(Vector<int>(0, -1));
  }
  else if (direction.y < 0) {
    for (int i=box.x / Tile::SIZE; i<=(box.x + box.w - 1) / Tile::SIZE; i++) {
      for (int j=facingPoint.y / Tile::SIZE; j>=destFacingPoint.y / Tile::SIZE; j--) {
        if (tileset[mTiles[i*mSize.y + j]].isObstacle()) {
          destFacingPoint.y = j * Tile::SIZE;
          break;
        }
      }
    }
    for (auto it=neighbours.begin(); it != neighbours.end(); it++) {
      Rect<float> &ebox = (**it).getBoundingBox();
      if (*it != &entity && (box.x + box.w > ebox.x && box.x < ebox.x + ebox.w))
        destFacingPoint.x = std::max(destFacingPoint.y, ebox.y + ebox.h);
    }
    box.y = destFacingPoint.y;
    if (box.y != dest.y)
      entity.onObstacleReached(Vector<int>(0, 1));
  }

  return box.x == dest.x && box.y == dest.y;
}

std::vector<Entity*> Level::getEntitiesInArea(const Rect<float> &area)
{
  std::vector<Entity*> entities;
  for (auto it=mEntities.begin(); it != mEntities.end(); it++) {
    if (area.intersects((**it).getBoundingBox()))
      entities.push_back(it->get());
  }

  return entities;
}

bool Level::isOnGround(Entity &entity)
{
  const Rect<float> &box = entity.getBoundingBox();

  // At the boundaries of the level ?
  if (box.y + box.h >= mSize.y * Tile::SIZE)
    return true;

  // Any solid tile below ?
  for (int i=box.x / Tile::SIZE; i <= (box.x + box.w - 1) / Tile::SIZE; i++) {
    if (tileset[mTiles[i*mSize.y + (box.y + box.h) / Tile::SIZE]].isObstacle())
      return true;
  }

  return false;
}

}
