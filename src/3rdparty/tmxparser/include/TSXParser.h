#ifndef TSXPARSER_H
#define TSXPARSER_H

#include "tmxcommon.h"
#include <string>
#include <vector>
#include <utility>
#include <map>

namespace TSX {
  class Parser
  {
    public:
      Parser( const char* filename );
      Parser();
      virtual ~Parser();

      bool load( const char* filename );

      struct Tileset {
        std::string name;
        unsigned int tileCount;
        unsigned int columns;
        unsigned int tileWidth;
        unsigned int tileHeight;
        unsigned int spacing;
        unsigned int margin;
        int offsetX;
        int offsetY;

        TMX::PropertyMap property;

        TMX::Image image;
      };

      struct Terrain {
        std::string name;
        unsigned int tile;
        TMX::PropertyMap property;
      };

      struct Tile {
        unsigned int id;
        TMX::Image image;
        std::vector<unsigned int> terrain;
        TMX::PropertyMap property;
        TMX::ObjectGroup collisions;
      };

      Tileset tileset;
      std::vector<Terrain> terrainList;
      std::vector<Tile> tileList;
  };
}
#endif // TSXPARSER_H
