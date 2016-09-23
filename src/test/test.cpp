#include "test.h"
#include "hero.h"
#include "world/level.h"
#include "world/tile.h"
#include "image.h"
#include "resource.h"
#include "log.h"
#include "enemy.h"
#include <cstring>
#include <memory>
#include <vector>

using namespace game;

TileID testTiles[] {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1,
};

const Vector<int> lvlSize(16*Tile::SIZE, 11*Tile::SIZE);

std::unique_ptr<Level> Test::makeLevel(Display& display)
{
  std::unique_ptr<Level> level = Level::loadFromTmx("assets/maps/test.tmx", display);

  // Platform
  level->addEntity(std::unique_ptr<Entity>(new Entity(150, 150, 50, 20, std::unique_ptr<Graphics>(new Rectangle(50, 20)), nullptr, true)));
  //level->addEntity(std::unique_ptr<Entity>(new Enemy(lvlSize.x - 100, lvlSize.y - (Tile::SIZE*2), Tile::SIZE, Tile::SIZE)));

  return level;
}
