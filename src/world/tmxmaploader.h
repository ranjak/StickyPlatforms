#ifndef TMXMAPLOADER_H
#define TMXMAPLOADER_H

#include "world/tile.h"
#include "gamevector.h"
#include "world/tileset.h"
#include <string>
#include <memory>
#include <vector>

namespace TMX {
class Parser;
}

namespace game {

class Level;
class Display;
class Entity;

class TMXMapLoader
{
public:

  static std::unique_ptr<Level> load(const std::string &file, Display &display);

private:
  TMXMapLoader();

  std::unique_ptr<Level> loadMap(const std::string &file, Display &display);

  void loadTiles(TMX::Parser &map, Display &display);

  void loadObjects(TMX::Parser &map, Level *level);

private:
  Vector<int> mLevelSize;
  // Ratio between the tile size of the level file and the fixed tile size of the game
  Vector<float> mTileRatio;
  TilesetList mTilesets;
  std::unique_ptr<TileID[]> mTilesArray;
};

namespace {
class MapCsvParser
{
public:
  MapCsvParser();

  bool loadTilesCsv(const std::string &tilesCsv, TileID *tilesArray, const Vector<int> &levelSize);

private:
  int readInt();

  void readWhitespace();

  void readSeparator();

  bool readChar();

private:
  size_t mCsvIndex;
  bool mCsvEnded;
  bool mCsvError;
  const std::string *mTilesCsv;
};
} // namespace

} // namespace game

#endif // TMXMAPLOADER_H
