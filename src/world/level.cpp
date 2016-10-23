#include "world/level.h"
#include "rectangle.h"
#include "log.h"
#include "rect.h"
#include "entity.h"
#include "gamestate.h"
#include "camera.h"
#include "TMXParser.h"
#include "world/tmxmaploader.h"
#include "movingphysicscomponent.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace game {


Level::Level(int width, int height, TilesetList &&tilesets, std::unique_ptr<TileID[]> tiles) :
  mSize(width, height),
  mTilesets(std::move(tilesets)),
  mTiles(std::move(tiles)),
  mEntities(*this),
  mHeroId(Entity::none)
{
}

std::unique_ptr<Level> Level::loadFromTmx(const std::string &file, Display &display)
{
  return TMXMapLoader::load(file, display);
}

void Level::update(GameState &game, uint32_t step)
{
  mEntities.update(step, game);
}

void Level::draw(Display &target, const GameState &game) const
{
  const Camera& cam = game.getCamera();
  Rect<float> viewport = cam.getViewport();

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

  mEntities.draw(target, game);
}

TileID *Level::tiles()
{
  return mTiles.get();
}

void Level::checkTileCollisions(MovingPhysicsComponent &object)
{
  Rect<float> box = object.entity().getGlobalBox();

  // Check every tile overlapping the entity as well as adjacent tiles
  int i = std::max<int>(0, box.x / Tile::SIZE - 1);
  int maxi = std::min<int>((box.x+box.w)/Tile::SIZE, mSize.x-1);

  int minj = std::max<int>(0, box.y / Tile::SIZE - 1);
  int maxj = std::min<int>((box.y+box.h)/Tile::SIZE, mSize.y-1);

  for (; i <= maxi; i++) {
    for (int j = minj; j <= maxj; j++) {

      TileID tile = mTiles[i*mSize.y + j];
      if (tile > 0 && mTilesets[tile].getCollisionBox(i, j).touches(box) && !object.isIgnored(Vector<int>(i,j))) {

        mTilesets[tile].onCollision(object.entity(), *this);
        object.collide(mTilesets[tile], Vector<int>(i, j));
      }
    }
  }
}

bool Level::start(const std::string &startingPoint)
{
  // Find the requested PlayerStart entity
  Entity *playerStart = mEntities.getEntity(startingPoint);

  if (!playerStart) {
    game::error("Level::start: starting point \""+startingPoint+"\" not found.");
    return false;
  }

  mHeroId = mEntities.makeEntity("Hero", "Hero", playerStart->getGlobalBox());
  assert(mHeroId != Entity::none);
  return true;
}

EntityManager &Level::entities()
{
  return mEntities;
}

const EntityManager &Level::entities() const
{
  return mEntities;
}

std::vector<Rect<float> > Level::getObstaclesInArea(Rect<float> area, const MovingPhysicsComponent &object) const
{
  // Make sure we're inside the level's boundaries
  area = area.getIntersection(Rect<float>(0.f, 0.f, mSize.x*Tile::SIZE, mSize.y*Tile::SIZE));

  if (!area.isValid())
    return std::vector<Rect<float>>();

  std::vector<Rect<float>> obstacles;

  for (int i=area.x/Tile::SIZE; i<=(area.x+area.w-1)/Tile::SIZE; i++) {
    for (int j=area.y/Tile::SIZE; j<=(area.y+area.h-1)/Tile::SIZE; j++) {

      TileID tile = mTiles[i*mSize.y + j];
      if (tile > 0 && mTilesets[tile].isObstacle() && !object.isIgnored(Vector<int>(i,j)))
        obstacles.push_back(mTilesets[tile].getCollisionBox(i, j));
    }
  }

  return obstacles;
}

Tile *Level::getTileAt(const Vector<int> &tilePos)
{
  return const_cast<Tile *>(static_cast<const Level &>(*this).getTileAt(tilePos));
}

const Tile *Level::getTileAt(const Vector<int> &tilePos) const
{
  assert(tilePos.x >= 0 && tilePos.x < mSize.x && tilePos.y >= 0 && tilePos.y < mSize.y);

  TileID tile = mTiles[tilePos.x*mSize.y + tilePos.y];

  return (tile == 0) ? nullptr : &mTilesets[tile];
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

Entity *Level::getHero()
{
  return mEntities.getEntity(mHeroId);
}

const Vector<int> &Level::getSize() const
{
  return mSize;
}

Vector<float> Level::getPixelSize() const
{
  return Vector<float>(mSize.x, mSize.y) * Tile::SIZE;
}

}
