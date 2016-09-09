#include "test.h"
#include "hero.h"
#include "platform.h"
#include <cstring>
#include <memory>

game::TileID testTiles[] {
  0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 1,
};

std::unique_ptr<game::Level> Test::makeLevel()
{
  std::unique_ptr<game::Level> level(new game::Level(10, 7));
  std::memcpy(level->tiles().get(), testTiles, 10 * 7 * sizeof(game::TileID));

  level->addEntity(std::unique_ptr<game::Entity>(new game::Hero));
  level->addEntity(std::unique_ptr<game::Entity>(new game::Platform(150, 150)));

  return level;
}
