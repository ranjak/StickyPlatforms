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

bool Level::tryMoving(Rect<float> &box, const Vector<float> &dest)
{
  Vector<float> direction(dest.x - box.x, dest.y - box.y);
  Vector<float> facingPoint;

  facingPoint.x = (direction.x > 0) ? box.x + box.w : box.x;
  facingPoint.y = (direction.y > 0) ? box.y + box.h : box.y;

  Vector<float> destFacingPoint(facingPoint.x + direction.x, facingPoint.y + direction.y);

  // Check obstacles on the X axis
  if (direction.x > 0) {
    // Find the closest tile obstacle on X
    for (int i=facingPoint.x / Tile::SIZE; i<=destFacingPoint.x / Tile::SIZE; i++) {
      for (int j=box.y / Tile::SIZE; j<=(box.y + box.h) / Tile::SIZE; j++) {
        if (tileset[mTiles[i*mSize.x + j]].isObstacle()) {
          destFacingPoint.x = i * Tile::SIZE;
          break;
        }
      }
    }
    // Move on the X axis up to the closest obstacle found
    box.x = destFacingPoint.x - box.w;
  }
  else {
    for (int i=facingPoint.x / Tile::SIZE; i>=destFacingPoint.x / Tile::SIZE; i--) {
      for (int j=box.y / Tile::SIZE; j<=(box.y + box.h) / Tile::SIZE; j++) {
        if (tileset[mTiles[i*mSize.x + j]].isObstacle()) {
          destFacingPoint.x = i * Tile::SIZE;
          break;
        }
      }
    }
    box.x = destFacingPoint.x;
  }

  // Same for the Y axis
  if (direction.y > 0) {
    for (int i=box.x / Tile::SIZE; i<=(box.x + box.w) / Tile::SIZE; i++) {
      for (int j=facingPoint.y / Tile::SIZE; j<=destFacingPoint.y / Tile::SIZE; j++) {
        if (tileset[mTiles[i*mSize.x + j]].isObstacle()) {
          destFacingPoint.y = j * Tile::SIZE;
          break;
        }
      }
    }
    box.y = destFacingPoint.y - box.h;
  }
  else {
    for (int i=box.x / Tile::SIZE; i<=(box.x + box.w) / Tile::SIZE; i++) {
      for (int j=facingPoint.y / Tile::SIZE; j>=destFacingPoint.y / Tile::SIZE; j--) {
        if (tileset[mTiles[i*mSize.x + j]].isObstacle()) {
          destFacingPoint.y = j * Tile::SIZE;
          break;
        }
      }
    }
    box.y = destFacingPoint.y;
  }

  return box.x == dest.x && box.y == dest.y;
}

}
