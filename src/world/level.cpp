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

}
