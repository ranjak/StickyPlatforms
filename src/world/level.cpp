#include "world/level.h"
#include "rectangle.h"
#include "log.h"
#include "hero.h"
#include "rect.h"
#include "entity.h"
#include "gamestate.h"
#include "camera.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace game {


Level::Level(int width, int height, std::unique_ptr<Hero> hero) :
  mSize(width, height),
  mTileset(),
  mTiles(),
  mEntities(),
  mHero(hero.get())
{
  if (width >= 0 && height >= 0)
    mTiles.reset(new TileID[width * height]);
  else
    throw std::runtime_error(std::string("Invalid level size: ") + std::to_string(width) + "x" + std::to_string(height));

  addEntity(std::move(hero));
}

void Level::update(GameState &game, uint32_t step)
{
  for (const std::unique_ptr<Entity>& entity : mEntities) {
    entity->update(step, game);
  }
  game.getCamera().update(step, game);
}

void Level::draw(Display &target, const GameState &game) const
{
  const Camera& cam = game.getCamera();
  const Rect<float>& viewport = cam.getBoundingBox();

  // Draw tiles. They are drawn by column.
  for (int i=viewport.x / Tile::SIZE; i<=(viewport.x+viewport.w - 1) / Tile::SIZE; i++) {
    for (int j=viewport.y / Tile::SIZE; j<=(viewport.y+viewport.h - 1) / Tile::SIZE; j++) {
      mTileset[mTiles[i*mSize.y + j]].draw(target, i, j, cam);
    }
  }

  // Draw entities
  for (const std::unique_ptr<Entity>& entity : mEntities) {
    if (entity->getBoundingBox().intersects(viewport))
      entity->draw(target, cam);
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

std::vector<Tile> &Level::tileset()
{
  return mTileset;
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

  // Store all entities that could possibly block our way
  Rect<float> movementArea;
  movementArea.x = std::min(destFacingPoint.x, box.x);
  movementArea.y = std::min(destFacingPoint.y, box.y);
  movementArea.w = (movementArea.x == box.x) ? (destFacingPoint.x - box.x) : (box.x + box.w - destFacingPoint.x);
  movementArea.h = (movementArea.y == box.y) ? (destFacingPoint.y - box.y) : (box.y + box.h - destFacingPoint.y);
  std::vector<Entity*> neighbours(getEntitiesInArea(movementArea));

  // Check obstacles on the X axis
  if (direction.x > 0) {
    // Find the closest entity obstacle on X
    for (auto it=neighbours.begin(); it != neighbours.end(); it++) {
      Rect<float> &ebox = (**it).getBoundingBox();
      if (*it != &entity && (box.y + box.h > ebox.y && box.y < ebox.y + ebox.h))
        destFacingPoint.x = std::min(destFacingPoint.x, ebox.x);
    }
    // Find the closest tile obstacle on X
    for (int i=facingPoint.x / Tile::SIZE; i<=(destFacingPoint.x - 1) / Tile::SIZE; i++) {
      for (int j=box.y / Tile::SIZE; j<=(box.y + box.h - 1) / Tile::SIZE; j++) {
        mTileset[mTiles[i*mSize.y + j]].onCollision(entity, *this);
        if (mTileset[mTiles[i*mSize.y + j]].isObstacle()) {
          destFacingPoint.x = i * Tile::SIZE;
          break;
        }
      }
    }
    // Move on the X axis up to the closest obstacle found
    box.x = destFacingPoint.x - box.w;
    // Notify the entity if it didn't reach its goal
    if (box.x != dest.x)
      entity.onObstacleReached(Vector<int>(-1, 0));
  }
  else if (direction.x < 0) {
    for (auto it=neighbours.begin(); it != neighbours.end(); it++) {
      Rect<float> &ebox = (**it).getBoundingBox();
      if (*it != &entity && (box.y + box.h > ebox.y && box.y < ebox.y + ebox.h))
        destFacingPoint.x = std::max(destFacingPoint.x, ebox.x + ebox.w);
    }
    for (int i=(int)facingPoint.x / Tile::SIZE; i>=(int)destFacingPoint.x / Tile::SIZE; i--) {
      for (int j=box.y / Tile::SIZE; j<=(box.y + box.h - 1) / Tile::SIZE; j++) {
        mTileset[mTiles[i*mSize.y + j]].onCollision(entity, *this);
        if (mTileset[mTiles[i*mSize.y + j]].isObstacle()) {
          destFacingPoint.x = std::min((i+1), mSize.x-1) * Tile::SIZE;
          break;
        }
      }
    }
    box.x = destFacingPoint.x;
    if (box.x != dest.x)
      entity.onObstacleReached(Vector<int>(1, 0));
  }

  // Same for the Y axis
  if (direction.y > 0) {
    for (auto it=neighbours.begin(); it != neighbours.end(); it++) {
      Rect<float> &ebox = (**it).getBoundingBox();
      if (*it != &entity && (box.x + box.w > ebox.x && box.x < ebox.x + ebox.w))
        destFacingPoint.y = std::min(destFacingPoint.y, ebox.y);
    }
    for (int i=box.x / Tile::SIZE; i<=(box.x + box.w - 1) / Tile::SIZE; i++) {
      for (int j=facingPoint.y / Tile::SIZE; j<=(destFacingPoint.y - 1) / Tile::SIZE; j++) {
        mTileset[mTiles[i*mSize.y + j]].onCollision(entity, *this);
        if (mTileset[mTiles[i*mSize.y + j]].isObstacle()) {
          destFacingPoint.y = j * Tile::SIZE;
          break;
        }
      }
    }
    box.y = destFacingPoint.y - box.h;
    if (box.y != dest.y)
      entity.onObstacleReached(Vector<int>(0, -1));
  }
  else if (direction.y < 0) {
    for (auto it=neighbours.begin(); it != neighbours.end(); it++) {
      Rect<float> &ebox = (**it).getBoundingBox();
      if (*it != &entity && (box.x + box.w > ebox.x && box.x < ebox.x + ebox.w))
        destFacingPoint.y = std::max(destFacingPoint.y, ebox.y + ebox.h);
    }
    for (int i=box.x / Tile::SIZE; i<=(box.x + box.w - 1) / Tile::SIZE; i++) {
      for (int j=(int)facingPoint.y / Tile::SIZE; j>=(int)destFacingPoint.y / Tile::SIZE; j--) {
        mTileset[mTiles[i*mSize.y + j]].onCollision(entity, *this);
        if (mTileset[mTiles[i*mSize.y + j]].isObstacle()) {
          destFacingPoint.y = std::min((j+1), mSize.y-1) * Tile::SIZE;
          break;
        }
      }
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
    if (mTileset[mTiles[i*mSize.y + (box.y + box.h) / Tile::SIZE]].isObstacle())
      return true;
  }

  // Any entity below ?
  for (auto it=mEntities.begin(); it != mEntities.end(); it++) {

    Rect<float> &obox = (**it).getBoundingBox();

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

        if (mTileset[mTiles[i*mSize.y + j]].isObstacle()
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

        if (mTileset[mTiles[i*mSize.y + j]].isObstacle()
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
