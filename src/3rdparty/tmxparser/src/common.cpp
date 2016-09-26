#include "common.h"
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <stdexcept>

namespace TMX {


std::string getDirectory(std::string filename)
{
  size_t filePos = filename.find_last_of("/\\");

  if (filePos != std::string::npos)
    filename.erase(filePos + 1);
  else
    filename.clear();

  return filename;
}

const char* presentOrDefault(rapidxml::xml_attribute<>* attribute, const char* defaultVal)
{
  return attribute != nullptr ? attribute->value() : defaultVal;
}

const char* findOrFail(rapidxml::xml_node<>* node, const char* attribute)
{
  rapidxml::xml_attribute<>* attr = node->first_attribute(attribute);

  if (attr != nullptr)
    return attr->value();

  else
    throw MissingAttributeError(node->name(), attribute);
}

TMX::Image parseImage(rapidxml::xml_node<>* img_node, const std::string &currentDir)
{
  Image image = {};

  image.source = currentDir + findOrFail( img_node, "source" );
  image.width = std::atoi( presentOrDefault( img_node->first_attribute( "width" ), "0" ) );
  image.height = std::atoi( presentOrDefault( img_node->first_attribute( "height" ), "0" ) );

  if( img_node->first_attribute( "trans" ) != 0 ) {
    image.transparentColor = std::atoi( img_node->first_attribute( "trans" )->value() );
  }

  return image;
}

Object parseObject(rapidxml::xml_node<> *obj_node)
{
  Object object = {};

  object.id = std::atoi(findOrFail(obj_node, "id"));
  object.name = presentOrDefault(obj_node->first_attribute("name"), "");
  object.type = presentOrDefault(obj_node->first_attribute("type"), "");
  object.visible = std::atoi(presentOrDefault(obj_node->first_attribute("visible"), "1"));
  object.x = std::atoi(findOrFail(obj_node, "x"));
  object.y = std::atoi(findOrFail(obj_node, "y"));
  object.rotation = std::atof(presentOrDefault(obj_node->first_attribute("rotation"), "0.0"));

  std::string pointList = "";
  // Determine the object's kind (tile, polygon, etc.)
  if (obj_node->first_attribute("gid")) {
    object.gid = std::atoi(obj_node->first_attribute("gid")->value());
    object.kind = Object::TILE;
  }
  else if (obj_node->first_node("ellipse")) {
    object.kind = Object::ELLIPSE;
  }
  else if (obj_node->first_node("polygon")) {
    object.kind = Object::POLYGON;
    pointList = findOrFail(obj_node->first_node("polygon"), "points");
  }
  else if (obj_node->first_node("polyline")) {
    object.kind = Object::POLYLINE;
    pointList = findOrFail(obj_node->first_node("polyline"), "points");
  }
  else {
    object.kind = Object::BOX;
  }

  // Parse polygon's and polyline's list of points
  if (object.kind == Object::POLYGON || object.kind == Object::POLYLINE) {

    size_t pos=0, nextPos=0;
    while (nextPos != std::string::npos) {

      nextPos = pointList.find(' ', pos);
      object.points.push_back(pointFromString(pointList.substr(pos, nextPos)));
      pos = nextPos + 1;
    }
  }
  else {
    object.width = std::atoi(findOrFail(obj_node, "width"));
    object.height = std::atoi(findOrFail(obj_node, "height"));
  }

  if (obj_node->first_node("properties"))
    object.property = parseProperties(obj_node->first_node("properties"));

  return object;
}

Point pointFromString(const std::string &point)
{
  size_t comma = point.find_first_of(",");

  if (comma == std::string::npos)
    throw std::runtime_error("Failed to parse Point structure from string \""+point+"\": missing comma separator");

  std::string x = point.substr(0, comma);
  std::string y = point.substr(comma+1);

  return Point { std::stoi(x), std::stoi(y) };
}

ObjectGroup parseObjectGroup(rapidxml::xml_node<> *group_node)
{
  ObjectGroup group = {};

  group.color = presentOrDefault( group_node->first_attribute( "color" ), "" );
  group.name = presentOrDefault(group_node->first_attribute("name"), "");
  group.opacity = std::atof( presentOrDefault( group_node->first_attribute( "opacity" ), "1.0" ) );
  group.visible = std::atoi( presentOrDefault( group_node->first_attribute( "visible" ), "1" ) );

  group.offsetx = std::atof( presentOrDefault( group_node->first_attribute( "offsetx" ), "0" ) );
  group.offsety = std::atof( presentOrDefault( group_node->first_attribute( "offsety" ), "0" ) );

  group.draworder = presentOrDefault( group_node->first_attribute( "draworder" ), "topdown" );

  if (group_node->first_node("properties"))
    group.property = parseProperties(group_node->first_node("properties"));

  // Parse every object in the group
  for (rapidxml::xml_node<> *objNode = group_node->first_node("object"); objNode != nullptr; objNode = objNode->next_sibling()) {
    group.objects.push_back(parseObject(objNode));
  }

  return group;
}

std::map<std::string, Property> parseProperties(rapidxml::xml_node<> *propertiesNode)
{
  std::map<std::string, Property> props;

  for(rapidxml::xml_node<>* pNode = propertiesNode->first_node("property"); pNode != nullptr; pNode = pNode->next_sibling()) {
    std::string name = findOrFail(pNode, "name");

    // The value may be stored in a "value" attribute or as content of the node.
    std::string value = (pNode->first_attribute("value")) ? pNode->first_attribute("value")->value() : pNode->value();

    std::string type = presentOrDefault(pNode->first_attribute("type"), "");
    Property::valType typev = Property::STRING;

    if (type == "int")
      typev = Property::INT;
    else if (type == "float")
      typev = Property::FLOAT;
    else if (type == "bool")
      typev = Property::BOOL;

    props.emplace(std::make_pair(name, Property(typev, value)));
  }

  return props;
}

}
