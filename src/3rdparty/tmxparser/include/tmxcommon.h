#ifndef TMXCOMMON_H
#define TMXCOMMON_H

#include <string>
#include <map>

namespace TMX {

struct Object {
  std::string name;
  std::string type;
  int x;
  int y;
  unsigned int width;
  unsigned int height;
  unsigned int gid;
  bool visible;
  std::map<std::string, std::string> property;
};

struct ObjectGroup {
  std::string color;
  std::string name;
  float opacity;
  bool visible;
  float offsetx;
  float offsety;
  std::string draworder;
  std::map<std::string, Object> object;
  std::map<std::string, std::string> property;
};

struct Image {
  std::string source;
  std::string transparentColor;
  unsigned int width;
  unsigned int height;
};

}

#endif // TMXCOMMON_H
