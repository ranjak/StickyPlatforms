#include "test.h"
#include "hero.h"
#include "platform.h"
#include "world/level.h"
#include "world/tile.h"
#include "image.h"
#include "resource.h"
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
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
};

std::unique_ptr<Level> Test::makeLevel(Display& display)
{
  std::unique_ptr<Level> level(new Level(16, 11, std::unique_ptr<Hero>(new Hero)));

  std::vector<Tile> &tset = level->tileset();

  tset.emplace_back(false);
  tset.emplace_back(true, std::make_shared<Image>(display, Resource::getInstance().assetPath("assets/PNG/grass_earth_tiles/grass_earth_05.png")));

  std::memcpy(level->tiles().get(), testTiles, 16 * 11 * sizeof(TileID));

  level->addEntity(std::unique_ptr<Entity>(new Platform(150, 150)));

  return level;
}
