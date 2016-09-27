#include "world/level.h"
#include "rectangle.h"
#include "log.h"
#include "hero.h"
#include "rect.h"
#include "entity.h"
#include "gamestate.h"
#include "camera.h"
#include "TMXParser.h"
#include "world/tmxmaploader.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace game {


Level::Level(int width, int height, std::unique_ptr<Hero> hero, TilesetList &&tilesets, std::unique_ptr<TileID[]> tiles) :
  mSize(width, height),
  mTilesets(std::move(tilesets)),
  mTiles(std::move(tiles)),
  mEntities(),
  mHero(hero.get())
{
  addEntity(std::move(hero));
}

std::unique_ptr<Level> Level::loadFromTmx(const std::string &file, Display &display)
{
  return TMXMapLoader::load(file, display);
}

void Level::update(GameState &game, uint32_t step)
{
  for (const std::unique_ptr<Entity>& entity : mEntities) {
    entity->update(step, game);
  }
  game.getCamera().update(step, game);

  mEntities.erase(std::remove_if(mEntities.begin(), mEntities.end(), [](std::unique_ptr<Entity>& entity) {return entity->isDead();}), mEntities.end());
}

void Level::draw(Display &target, const GameState &game) const
{
  const Camera& cam = game.getCamera();
  Rect<float> viewport = cam.getGlobalBox();

  // The camera might be too big for the level
  viewport.w = std::min(static_cast<int>(viewport.w), mSize.x * Tile::SIZE);
  viewport.h = std::min(static_cast<int>(viewport.h), mSize.y * Tile::SIZE);

  // Draw tiles. They are drawn by column.
  for (int i=viewport.x / Tile::SIZE; i<=(viewport.x+viewport.w - 1) / Tile::SIZE; i++) {
    for (int j=viewport.y / Tile::SIZE; j<=(viewport.y+viewport.h - 1) / Tile::SIZE; j++) {
      TileID &tile = mTiles[i*mSize.y + j];
      if (tile > 0)
        mTilesets[tile].draw(target, i, j, cam);
    }
  }

  // Draw entities
  for (const std::unique_ptr<Entity>& entity : mEntities) {
    if (entity->getGlobalBox().intersects(viewport))
      entity->draw(target, cam);
  }
}

void Level::addEntity(std::unique_ptr<Entity> entity)
{
  mEntities.push_back(std::move(entity));
}

TileID *Level::tiles()
{
  return mTiles.get();
}

void Level::handleCollisions(Entity &entity)
{
  Rect<float> box = entity.getGlobalBox();

  // Check tile collisions first
  for (int i=box.x / Tile::SIZE; i<=(box.x+box.w - 1)/Tile::SIZE; i++) {
    for (int j=box.y / Tile::SIZE; j<=(box.y+box.h - 1)/Tile::SIZE; j++) {

      if (mTiles[i*mSize.y + j] > 0) {
        mTilesets[mTiles[i*mSize.y + j]].onCollision(entity, *this);
        entity.onCollision(mTilesets[mTiles[i*mSize.y + j]], Vector<int>(i*Tile::SIZE, j*Tile::SIZE));
      }
    }
  }

  // Check entity collisions
  for (const std::unique_ptr<Entity> &other : mEntities) {
    if (other.get() != &entity && other->isCollidable() && box.touches(other->getGlobalBox())) {
      other->onCollision(entity);
      entity.onCollision(*other);
    }
  }
}

bool Level::tryMoving(Entity &entity, const Vector<float> &dest)
{
  Rect<float> box = entity.getGlobalBox();
  Vector<float> direction(dest.x - box.x, dest.y - box.y);
  Vector<float> facingPoint;

  facingPoint.x = (direction.x >= 0) ? box.x + box.w : box.x;
  facingPoint.y = (direction.y >= 0) ? box.y + box.h : box.y;

  Vector<float> destFacingPoint(facingPoint.x + direction.x, facingPoint.y + direction.y);

  // Do not go beyond the level's boundaries
  if (destFacingPoint.x < 0)
    destFacingPoint.x = 0;
  else if (destFacingPoint.x > mSize.x * Tile::SIZE)
    destFacingPoint.x = mSize.x * Tile::SIZE;

  if (destFacingPoint.y < 0)
    destFacingPoint.y = 0;
  else if (destFacingPoint.y > mSize.y * Tile::SIZE)
    destFacingPoint.y = mSize.y * Tile::SIZE;

  if (!entity.isCollidable() || entity.ignoresObstacles()) {

    box.x = (direction.x >= 0) ? destFacingPoint.x-box.w : destFacingPoint.x;
    box.y = (direction.y >= 0) ? destFacingPoint.y-box.h : destFacingPoint.y;

    entity.setGlobalPos(Vector<float>(box.x, box.y));
    return box.x == dest.x && box.y == dest.y;
  }

  // Store all entities that could possibly block our way
  Rect<float> movementArea;
  movementArea.x = std::min(destFacingPoint.x, box.x);
  movementArea.y = std::min(destFacingPoint.y, box.y);
  movementArea.w = (movementArea.x == box.x) ? (destFacingPoint.x - box.x) : (box.x + box.w - destFacingPoint.x);
  movementArea.h = (movementArea.y == box.y) ? (destFacingPoint.y - box.y) : (box.y + box.h - destFacingPoint.y);
  std::vector<Entity*> neighbours(getEntitiesInArea(movementArea, [&](const Entity &e) { return &entity != &e && e.isObstacle(); }));

  // Also check tile collisions
  std::vector<Rect<float>> obstacles;

  for (int i=movementArea.x/Tile::SIZE; i<=(movementArea.x+movementArea.w-1)/Tile::SIZE; i++) {
    for (int j=movementArea.y/Tile::SIZE; j<=(movementArea.y+movementArea.h-1)/Tile::SIZE; j++) {

      TileID tile = mTiles[i*mSize.y + j];
      if (tile > 0 && mTilesets[tile].isObstacle())
        obstacles.push_back(mTilesets[tile].getCollisionBox(i, j));
    }
  }

  std::for_each(neighbours.begin(), neighbours.end(), [&](const Entity *e) { obstacles.push_back(e->getGlobalBox()); });

  // Check obstacles on the X axis
  if (direction.x > 0) {
    // Find the closest entity obstacle on X
    for (auto it=obstacles.begin(); it != obstacles.end(); it++) {
      if (box.x < it->x && box.y + box.h > it->y && box.y < it->y + it->h)
        destFacingPoint.x = std::min(destFacingPoint.x, it->x);
    }

    // Move on the X axis up to the closest obstacle found
    box.x = destFacingPoint.x - box.w;
    // Notify the entity if it didn't reach its goal
    if (box.x != dest.x)
      entity.onObstacleReached(Vector<int>(-1, 0));
  }
  else if (direction.x < 0) {
    for (auto it=obstacles.begin(); it != obstacles.end(); it++) {
      if (box.x > it->x && box.y + box.h > it->y && box.y < it->y + it->h)
        destFacingPoint.x = std::max(destFacingPoint.x, it->x + it->w);
    }

    box.x = destFacingPoint.x;
    if (box.x != dest.x)
      entity.onObstacleReached(Vector<int>(1, 0));
  }

  // Same for the Y axis
  if (direction.y > 0) {
    for (auto it=obstacles.begin(); it != obstacles.end(); it++) {
      if (box.y < it->y && box.x + box.w > it->x && box.x < it->x + it->w)
        destFacingPoint.y = std::min(destFacingPoint.y, it->y);
    }

    box.y = destFacingPoint.y - box.h;
    if (box.y != dest.y)
      entity.onObstacleReached(Vector<int>(0, -1));
  }
  else if (direction.y < 0) {
    for (auto it=obstacles.begin(); it != obstacles.end(); it++) {
      if (box.y > it->y && box.x + box.w > it->x && box.x < it->x + it->w)
        destFacingPoint.y = std::max(destFacingPoint.y, it->y + it->h);
    }

    box.y = destFacingPoint.y;
    if (box.y != dest.y)
      entity.onObstacleReached(Vector<int>(0, 1));
  }

  entity.setGlobalPos(Vector<float>(box.x, box.y));
  return box.x == dest.x && box.y == dest.y;
}

template<typename Func>
std::vector<Entity*> Level::getEntitiesInArea(const Rect<float> &area, Func &&pred)
{
  std::vector<Entity*> entities;
  for (auto it=mEntities.begin(); it != mEntities.end(); it++) {
    if (pred(**it) && area.intersects((**it).getGlobalBox()))
      entities.push_back(it->get());
  }

  return entities;
}

bool Level::isOnGround(Entity &entity)
{
  Rect<float> box = entity.getGlobalBox();

  // At the boundaries of the level ?
  if (box.y + box.h >= mSize.y * Tile::SIZE)
    return true;

  // Any solid tile below ?
  for (int i=box.x / Tile::SIZE; i <= (box.x + box.w - 1) / Tile::SIZE; i++) {

    TileID &tile = mTiles[i*mSize.y + (box.y + box.h) / Tile::SIZE];
    if (tile > 0 && mTilesets[tile].isObstacle())
      return true;
  }

  // Any entity below ?
  for (auto it=mEntities.begin(); it != mEntities.end(); it++) {

    Rect<float> obox = (**it).getGlobalBox();

    if (it->get() != &entity && (box.y + box.h == obox.y) && (box.x + box.w > obox.x) && (box.x < obox.x + obox.w))
      return true;
  }

  return false;
}

bool Level::getFacingObstacle(const Rect<float> &box, const Vector<float> &direction, Vector<int> &obstacle, int maxPoint)
{
  // This function works for only a single coordinate at a time
  assert(!(direction.x != 0 && direction.y != 0) && !(direction.x == 0 && direction.y == 0));

  // Get the outside facing point of the box for the given direction
  int facingPoint;

  if (direction.x < 0) {
    facingPoint = box.x - 1;
    maxPoint = std::max(0, maxPoint);
  }
  else if (direction.x > 0) {
    facingPoint = box.x + box.h;
    maxPoint = std::min(mSize.x*Tile::SIZE - 1, maxPoint);
  }
  else if (direction.y < 0) {
    facingPoint = box.y - 1;
    maxPoint = std::max(0, maxPoint);
  }
  else if (direction.y > 0) {
    facingPoint = box.y + box.h;
    maxPoint = std::min(mSize.y*Tile::SIZE - 1, maxPoint);
  }

  // Start and end points to iterate over the tile array, depend on the direction
  std::pair<int,int> indices = std::minmax(facingPoint, maxPoint);
  indices.first /= Tile::SIZE;
  indices.second /= Tile::SIZE;

  bool obstacleFound = false;
  obstacle.x = obstacle.y = maxPoint;

  if (direction.x != 0) {
    for (int i=indices.first; i<=indices.second; i++) {
      for (int j=box.y / Tile::SIZE; j<=(box.y+box.h - 1) / Tile::SIZE; j++) {

        TileID &tile = mTiles[i*mSize.y + j];

        if (tile > 0 && mTilesets[tile].isObstacle()
            && std::abs(obstacle.x - facingPoint) >= std::abs(i*Tile::SIZE - facingPoint)) {

          obstacle.x = i*Tile::SIZE;
          obstacle.y = j*Tile::SIZE;
          obstacleFound = true;
        }
      }
    }
  }
  else {
    for (int i=box.y / Tile::SIZE; i<=(box.y+box.h - 1) / Tile::SIZE; i++) {
      for (int j=indices.first; j<=indices.second; j++) {

        TileID &tile = mTiles[i*mSize.y + j];

        if (tile > 0 && mTilesets[tile].isObstacle()
            && std::abs(obstacle.y - facingPoint) >= std::abs(j*Tile::SIZE - facingPoint)) {

          obstacle.x = i*Tile::SIZE;
          obstacle.y = j*Tile::SIZE;
          obstacleFound = true;
        }
      }
    }
  }

  return obstacleFound;
}

bool Level::getFacingObstacle(const Rect<float> &box, const Vector<float> &direction, Vector<int> &obstacle)
{
  // Set maxPoint to the level's boundary pointed to by the direction
  int maxPoint;

  if (direction.x < 0 || direction.y < 0)
    maxPoint = 0;
  else if (direction.x > 0)
    maxPoint = mSize.x * Tile::SIZE - 1;
  else if (direction.y > 0)
    maxPoint = mSize.y * Tile::SIZE - 1;
  else
    assert(false);

  return getFacingObstacle(box, direction, obstacle, maxPoint);
}

Hero *Level::getHero()
{
  return mHero;
}

const Vector<int> &Level::getSize()
{
  return mSize;
}

Vector<int> Level::getPixelSize()
{
  return mSize * Tile::SIZE;
}

}
