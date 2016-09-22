#include "world/tmxmaploader.h"
#include "world/level.h"
#include "world/tile.h"
#include "TMXParser.h"
#include "hero.h"
#include "log.h"
#include <string>
#include <memory>
#include <cctype>

namespace game {


std::unique_ptr<Level> TMXMapLoader::load(const std::string &file)
{
  TMXMapLoader loader;

  TMX::Parser tmx(file.c_str());

  std::unique_ptr<Level> level(new Level(tmx.mapInfo.width, tmx.mapInfo.height, std::unique_ptr<Hero>(new Hero)));

  // TODO load tilesets

  // Fill the tile array
  std::map<std::string, TMX::Parser::TileLayer>::iterator tiles = tmx.tileLayer.find("tiles");
  if (tiles == tmx.tileLayer.end()) {
    game::error("Level::loadFromTmx: A tile layer with name 'tiles' is required.");
    return nullptr;
  }

  if (!loader.loadTilesCsv(tiles->second.data.contents, *level)) {
    game::error("Level::loadFromTmx: CSV tile data is corrupt.");
    return nullptr;
  }

  return level;
}

TMXMapLoader::TMXMapLoader() :
  mCsvIndex(0),
  mCsvEnded(false),
  mCsvError(false),
  mTilesCsv()
{

}

bool TMXMapLoader::loadTilesCsv(const std::string &tilesCsv, Level &level)
{
  mTilesCsv = &tilesCsv;
  mCsvEnded = false;
  mCsvError = false;
  mCsvIndex = 0;

  Vector<int> size = level.getSize();
  TileID *tiles = level.tiles();

  int i=0, j=0;

  readWhitespace();

  for (j=0; j<size.y && !mCsvError && !mCsvEnded; j++) {

    for (i=0; i<size.x && !mCsvError && !mCsvEnded; i++) {

      tiles[i*size.y + j] = readInt();

      if (!mCsvEnded)
        readSeparator();
    }
  }

  // Check whether the whole array was filled
  return i == size.x && j == size.y;
}

int TMXMapLoader::readInt()
{
  std::string number = "";

  while (!mCsvEnded && ('0' <= mTilesCsv->at(mCsvIndex)) && (mTilesCsv->at(mCsvIndex) <= '9')) {
    number += mTilesCsv->at(mCsvIndex);
    readChar();
  }

  if (!number.empty()) {
    return std::atoi(number.c_str());
  }
  else {
    mCsvError = true;
    return 0;
  }
}

void TMXMapLoader::readWhitespace()
{
  while (!mCsvEnded && std::isspace(static_cast<int>(mTilesCsv->at(mCsvIndex))))
    readChar();
}

bool TMXMapLoader::readChar()
{
  if (++mCsvIndex < mTilesCsv->size()) {
    return true;
  }
  else {
    mCsvEnded = true;
    return false;
  }
}

void TMXMapLoader::readSeparator()
{
  // Read any whites before
  readWhitespace();

  if (!mCsvEnded && mTilesCsv->at(mCsvIndex) == ',') {
    readChar();
    // Read any whites after
    readWhitespace();
  }
  else {
    mCsvError = true;
  }
}

} // namespace game
