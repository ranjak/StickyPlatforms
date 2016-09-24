#ifndef TILESET_H
#define TILESET_H

#include "world/tile.h"
#include "gamevector.h"
#include <vector>
#include <utility>

namespace game {

class Tileset
{
public:
  Tileset(const std::string &name, bool isSingleImage, Vector<int> tileSize, std::vector<Tile> &&tiles);

  Tile &operator[] (TileID id);
  const Tile &operator[](TileID id) const;

  int numTiles() const { return mTiles.size(); }

  int maxId() const { return mMaxId; }

public:
  const std::string name;

private:
  bool mSingleImage;
  std::vector<Tile> mTiles;
  Vector<int> mOffset;
  Vector<int> mTileSize;
  int mMaxId;
};

class TilesetList
{
public:
  TilesetList() = default;
  /**
   * @brief operator [] Access a tile in this collection of tilesets using its global ID.
   * @param gid Global ID of the tile. Global IDs are specific to a particular map.
   * @return The tile at the given global ID.
   */
  Tile &operator[](TileID gid);
  const Tile &operator[](TileID gid) const;

  /**
   * @brief add Add a tileset to this collection.
   * @param tileset Tileset to be added.
   * @param firstGid Map-specific Global ID at which this tileset starts.
   */
  void add(Tileset &&tileset, TileID firstGid);

private:
  // Pairs associating tileset with its firstgid in the map.
  std::vector<std::pair<TileID, Tileset>> mTilesets;
};

} // namespace game

#endif // TILESET_H
