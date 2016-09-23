#include "tsxtilesetloader.h"
#include "log.h"
#include "world/tile.h"
#include "world/tileset.h"
#include "TSXParser.h"
#include "image.h"
#include "rect.h"
#include <string>
#include <vector>
#include <stdexcept>

namespace game {

TSXTilesetLoader::TSXTilesetLoader()
{

}

Tileset TSXTilesetLoader::loadTileset(const std::string &file, Display &display)
{
  TSX::Parser tsx;

  if (!tsx.load(file.c_str())) {
    game::error("Failed to load tileset file "+file);
    throw std::runtime_error("Failed to load tileset file "+file);
  }

  bool singleImage = !tsx.tileset.image.source.empty();
  std::shared_ptr<Image> tilesetImg;

  Vector<int> tileSize(tsx.tileset.tileWidth, tsx.tileset.tileHeight);

  if (singleImage)
    tilesetImg.reset(new Image(display, tsx.tileset.image.source));

  // Make Tile objects
  std::vector<Tile> tiles;

  // TODO handle "special" tiles
  if (singleImage) {
    for (size_t i=0; i<tsx.tileset.tileCount; i++) {
      Rect<int> imgPos((i*tileSize.x)%tilesetImg->size().x, (i*tileSize.x)/tilesetImg->size().x, tileSize.x, tileSize.y);

      tiles.emplace_back(i, true, tilesetImg, imgPos);
    }
  }
  else {
    for (TSX::Parser::Tile &tile : tsx.tileList) {
      std::shared_ptr<Image> tileImg = std::make_shared<Image>(display, tile.image.source);

      tiles.emplace_back(tile.id, true, tileImg);
    }
  }

  return Tileset(tsx.tileset.name, singleImage, tileSize, std::move(tiles));
}

} // namespace game