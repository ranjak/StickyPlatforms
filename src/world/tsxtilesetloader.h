#ifndef TSXTILESETLOADER_H
#define TSXTILESETLOADER_H

#include <iosfwd>
#include <memory>

namespace game {

class Tileset;
class Display;

class TSXTilesetLoader
{
public:
  static Tileset loadTileset(const std::string &file, Display &display);
private:
  TSXTilesetLoader();
};

}

#endif // TSXTILESETLOADER_H
