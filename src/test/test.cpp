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
  std::unique_ptr<Level> level(new Level(16, 11, std::unique_ptr<Hero>(new Hero)));

  // Fill the tileset with tiles to match the IDs in testTiles
  std::vector<Tile> &tset = level->tileset();

  tset.emplace_back(false);
  tset.emplace_back(true, std::make_shared<Image>(display, "assets/PNG/grass_earth_tiles/grass_earth_05.png"));
  tset.emplace_back(false,
                    std::shared_ptr<Graphics>(),
                    [] (Entity &entity, Level &level) { if (&entity == level.getHero()) Log::getGlobal().log(Log::INFO, "Victory!"); });

  // Fille the level's tile array with testTiles
  std::memcpy(level->tiles().get(), testTiles, 16 * 11 * sizeof(TileID));

  // Platform
  level->addEntity(std::unique_ptr<Entity>(new Entity(150, 150, 50, 20, std::unique_ptr<Graphics>(new Rectangle(50, 20)), nullptr, true)));
  level->addEntity(std::unique_ptr<Entity>(new Enemy(lvlSize.x - 100, lvlSize.y - (Tile::SIZE*2), Tile::SIZE, Tile::SIZE)));

  return level;
}
