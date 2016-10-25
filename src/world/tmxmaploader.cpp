#include "world/tmxmaploader.h"
#include "world/level.h"
#include "world/tile.h"
#include "world/tileset.h"
#include "TMXParser.h"
#include "log.h"
#include "tsxtilesetloader.h"
#include "entityfactory.h"
#include "damagecomponent.h"
#include "make_unique.h"
#include "entitygroup.h"
#include <string>
#include <memory>
#include <cctype>
#include <vector>
#include <stdexcept>

namespace game {


std::unique_ptr<Level> TMXMapLoader::load(const std::string &file, Display &display)
{
  try {
    return TMXMapLoader().loadMap(file, display);
  }
  catch (std::exception &e) {
    game::error("Failed to load map file "+file+" ; reason: "+e.what());
    return nullptr;
  }
}


TMXMapLoader::TMXMapLoader() :
  mLevelSize(),
  mTileRatio(),
  mTilesets(),
  mTilesArray()
{

}

std::unique_ptr<Level> TMXMapLoader::loadMap(const std::string &file, Display &display)
{
  TMX::Parser tmx(file.c_str());

  mLevelSize.x = tmx.mapInfo.width;
  mLevelSize.y = tmx.mapInfo.height;

  mTileRatio.x = static_cast<float>(Tile::SIZE) / static_cast<float>(tmx.mapInfo.tileWidth);
  mTileRatio.y = static_cast<float>(Tile::SIZE) / static_cast<float>(tmx.mapInfo.tileHeight);

  loadTiles(tmx, display);

  std::unique_ptr<Level> level(new Level(mLevelSize.x, mLevelSize.y, std::move(mTilesets), std::move(mTilesArray)));

  loadObjects(tmx, *level);


  return level;
}

void TMXMapLoader::loadTiles(TMX::Parser &map, Display &display)
{
  // Load the map's tilesets
  for (TMX::Parser::Tileset &tileset : map.tilesetList) {
    mTilesets.add(TSXTilesetLoader::loadTileset(tileset.source, display), tileset.firstGID);
  }

  // Fill the tile array
  mTilesArray.reset(new TileID[mLevelSize.x * mLevelSize.y]);

  std::map<std::string, TMX::Parser::TileLayer>::iterator tiles = map.tileLayer.find("tiles");

  if (tiles == map.tileLayer.end())
    throw std::runtime_error("Level::loadFromTmx: A tile layer with name 'tiles' is required.");

  if (!MapCsvParser().loadTilesCsv(tiles->second.data.contents, mTilesArray.get(), mLevelSize))
    throw std::runtime_error("Level::loadFromTmx: CSV tile data is corrupt.");
}

void TMXMapLoader::loadObjects(TMX::Parser &map, Level &level)
{
  auto mapEntities = map.objectGroup.find("entities");

  bool hasPlayerStart = false;

  if (mapEntities == map.objectGroup.end())
    throw std::runtime_error("Could not find required \"entities\" object layer in the map file.");

  for (TMX::Object &obj : mapEntities->second.objects) {

    EntityID entity = level.entities().makeEntity(obj.type, obj.name, Rect<float>(obj.x * mTileRatio.x, obj.y * mTileRatio.y, obj.width * mTileRatio.x, obj.height * mTileRatio.y));

    if (entity != Entity::none && !hasPlayerStart)
      hasPlayerStart = (obj.name == "playerStart");
  }

  if (!hasPlayerStart)
    throw std::runtime_error("Could not find required playerStart object in the map file.");

  // Place solid boxes around the map for proper collision handling at the edges
  Vector<float> size = level.getPixelSize();

  EntityID bottom = level.entities().makeEntity("invisibleWall", "bottom", Rect<float>(0.f, size.y, size.x, 10.f));
  level.entities().makeEntity("invisibleWall", "top", Rect<float>(0.f, -10.f, size.x, 10.f));
  level.entities().makeEntity("invisibleWall", "left", Rect<float>(-10.f, 0.f, 10.f, size.y));
  level.entities().makeEntity("invisibleWall", "right", Rect<float>(size.x, 0.f, 10.f, size.y));

  // Make the bottom of the level deadly
  level.entities().getEntity(bottom)->addComponent(std::make_unique<DamageComponent>(99, EntityGroup::ANY, true));
}

namespace {

MapCsvParser::MapCsvParser() :
  mCsvIndex(0),
  mCsvEnded(false),
  mCsvError(false),
  mTilesCsv()
{

}

bool MapCsvParser::loadTilesCsv(const std::string &tilesCsv, TileID *tilesArray, const Vector<int> &levelSize)
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

int MapCsvParser::readInt()
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

void MapCsvParser::readWhitespace()
{
  while (!mCsvEnded && std::isspace(static_cast<int>(mTilesCsv->at(mCsvIndex))))
    readChar();
}

bool MapCsvParser::readChar()
{
  if (++mCsvIndex < mTilesCsv->size()) {
    return true;
  }
  else {
    mCsvEnded = true;
    return false;
  }
}

void MapCsvParser::readSeparator()
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

} // namespace

} // namespace game
