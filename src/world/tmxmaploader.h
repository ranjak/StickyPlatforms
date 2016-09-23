#ifndef TMXMAPLOADER_H
#define TMXMAPLOADER_H

#include <string>
#include <memory>
#include "world/tile.h"

namespace game {

class Level;
class Display;

class TMXMapLoader
{
public:

  static std::unique_ptr<Level> load(const std::string &file, Display &display);

private:
  TMXMapLoader();

  bool loadTilesCsv(const std::string &tilesCsv, TileID *tilesArray, const Vector<int> &levelSize);

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

} // namespace game

#endif // TMXMAPLOADER_H
