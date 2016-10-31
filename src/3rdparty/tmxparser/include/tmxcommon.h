#ifndef TMXCOMMON_H
#define TMXCOMMON_H

#include <string>
#include <map>
#include <vector>
#include <new>

namespace TMX {


struct Point {
  int x;
  int y;
};

struct Property {
  enum valType { STRING, INT, FLOAT, BOOL } type;
  union
  {
    std::string strVal;
    int intVal;
    float floatVal;
    bool boolVal;
  };

  Property(valType type, const std::string &value);

  ~Property();

  Property(const Property &p);
  Property &operator=(const Property &p);

  Property(Property &&p);
  Property &operator=(Property &&p);
};

struct PropertyMap {
  std::map<std::string, Property> properties;

  const Property *find(const std::string &name, Property::valType type) const;

  bool getBool(const std::string &name, bool &value, bool defaultVal=false) const;
  bool getFloat(const std::string &name, float &value, float defaultVal=0.f) const;
  bool getInt(const std::string &name, int &value, int defaultVal=0) const;
  bool getString(const std::string &name, std::string &value, const std::string &defaultVal="") const;
};

struct Object {
  int id;
  std::string name;
  std::string type;
  int x;
  int y;
  unsigned int width;
  unsigned int height;
  bool visible;
  float rotation;

  // Used when the object is associated with a tile
  unsigned int gid;
  // Used by polygons and polylines
  std::vector<Point> points;

  enum { TILE, BOX, ELLIPSE, POLYGON, POLYLINE } kind;
  PropertyMap property;
};

struct ObjectGroup {
  std::string color;
  std::string name;
  float opacity;
  bool visible;
  float offsetx;
  float offsety;
  std::string draworder;
  std::vector<Object> objects;
  PropertyMap property;
};

struct Image {
  std::string source;
  std::string transparentColor;
  unsigned int width;
  unsigned int height;
};


}

#endif // TMXCOMMON_H
