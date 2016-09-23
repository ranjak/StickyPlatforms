#include "world/tmxmaploader.h"
#include "world/level.h"
#include "world/tile.h"
#include "world/tileset.h"
#include "TMXParser.h"
#include "hero.h"
#include "log.h"
#include "tsxtilesetloader.h"
#include <string>
#include <memory>
#include <cctype>

namespace game {


std::unique_ptr<Level> TMXMapLoader::load(const std::string &file, Display &display)
{
  TMXMapLoader loader;

  TMX::Parser tmx(file.c_str());

  // Load the map's tilesets
  TilesetList tilesets;
  for (TMX::Parser::Tileset tileset : tmx.tilesetList) {
    tilesets.add(TSXTilesetLoader::loadTileset(tileset.source, display), tileset.firstGID);
  }

  // Fill the tile array
  Vector<int> levelSize(tmx.mapInfo.width, tmx.mapInfo.height);
  std::unique_ptr<TileID[]> tilesArray(new TileID[levelSize.x * levelSize.y]);

  std::map<std::string, TMX::Parser::TileLayer>::iterator tiles = tmx.tileLayer.find("tiles");
  if (tiles == tmx.tileLayer.end()) {
    game::error("Level::loadFromTmx: A tile layer with name 'tiles' is required.");
    return nullptr;
  }

  if (!loader.loadTilesCsv(tiles->second.data.contents, tilesArray.get(), levelSize)) {
    game::error("Level::loadFromTmx: CSV tile data is corrupt.");
    return nullptr;
  }

  std::unique_ptr<Level> level(new Level(levelSize.x, levelSize.y, std::unique_ptr<Hero>(new Hero), std::move(tilesets), std::move(tilesArray)));
  return level;
}

TMXMapLoader::TMXMapLoader() :
  mCsvIndex(0),
  mCsvEnded(false),
  mCsvError(false),
  mTilesCsv()
{

}

bool TMXMapLoader::loadTilesCsv(const std::string &tilesCsv, TileID *tilesArray, const Vector<int> &levelSize)
{
  mTilesCsv = &tilesCsv;
  mCsvEnded = false;
  mCsvError = false;
  mCsvIndex = 0;

  int i=0, j=0;

  readWhitespace();

  for (j=0; j<levelSize.y && !mCsvError && !mCsvEnded; j++) {

    for (i=0; i<levelSize.x && !mCsvError && !mCsvEnded; i++) {

      tilesArray[i*levelSize.y + j] = readInt();

      if (!mCsvEnded)
        readSeparator();
    }
  }

  // Check whether the whole array was filled
  return i == levelSize.x && j == levelSize.y;
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
