#include "tileset.h"
#include "gamevector.h"
#include "log.h"
#include <string>
#include <stdexcept>
#include <cassert>

namespace game {

// TODO handle draw offset
Tileset::Tileset(const std::string &name, bool isSingleImage, Vector<int> tileSize, std::vector<Tile> &&tiles) :
  name(name),
  mSingleImage(isSingleImage),
  mTiles(std::move(tiles)),
  mOffset(),
  mTileSize(tileSize)
{
}

Tile &Tileset::operator[](TileID id)
{
  return const_cast<Tile &>(static_cast<const Tileset &>(*this)[id]);
}

const Tile &Tileset::operator[](TileID id) const
{
  int tilecount = numTiles();

  if (mSingleImage) {
    assert(id < tilecount);
    return mTiles[id];
  }

  // For multi-image tilesets, ids are not always consecutive
  for (int i=0; i<tilecount; i++) {
    if (mTiles[i].id == id)
      return mTiles[i];
  }

  game::error("No tile with ID "+std::to_string(id)+" in tileset "+name);
  throw std::runtime_error("No tile with ID "+std::to_string(id)+" in tileset "+name);
}


Tile &TilesetList::operator[](TileID gid)
{
  return const_cast<Tile &>(static_cast<const TilesetList &>(*this)[gid]);
}

const Tile &TilesetList::operator[](TileID gid) const
{
  for (const std::pair<TileID, Tileset> &tileset : mTilesets) {
    if (tileset.first <= gid && (gid - tileset.first) < tileset.second.numTiles())
      return tileset.second[gid - tileset.first];
  }

  game::error("No tile with GID "+std::to_string(gid)+" in this tileset collection");
  throw std::runtime_error("No tile with GID "+std::to_string(gid)+" in this tileset collection");
}

void TilesetList::add(Tileset &&tileset, TileID firstGid)
{
  mTilesets.push_back(std::make_pair(firstGid, std::move(tileset)));
}

} // namespace game
