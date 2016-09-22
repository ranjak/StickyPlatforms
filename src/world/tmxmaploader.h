#ifndef TMXMAPLOADER_H
#define TMXMAPLOADER_H

#include <string>
#include <memory>

namespace game {

class Level;

class TMXMapLoader
{
public:

  static std::unique_ptr<Level> load(const std::string &file);

private:
  TMXMapLoader();

  bool loadTilesCsv(const std::string &tilesCsv, Level &level);

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
