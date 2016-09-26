#ifndef COMMON_H
#define COMMON_H

#include "tmxcommon.h"
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <map>


namespace TMX {


std::string getDirectory(std::string filename);

const char* findOrFail(rapidxml::xml_node<>* node, const char* attribute);

const char* presentOrDefault(rapidxml::xml_attribute<>* attribute, const char* defaultVal);

/**
 * @brief parseImage Parse an image node, storing its characteristics in an Image struct.
 * @param img_node XML node of the image.
 * @param currentDir Directory containing the file being parsed (used to correct the image's source attribute).
 * @return Image struct with parsed data.
 */
Image parseImage(rapidxml::xml_node<>* img_node, const std::string &currentDir);

ObjectGroup parseObjectGroup(rapidxml::xml_node<>* group_node);

Object parseObject(rapidxml::xml_node<>* obj_node);

std::map<std::string, Property> parseProperties(rapidxml::xml_node<>* propertiesNode);

Point pointFromString(const std::string &point);

class MissingAttributeError : public std::exception
{
public:
  MissingAttributeError(std::string node, std::string attribute) :
    std::exception(),
    message("Missing required "+node+" attribute: "+attribute)
  {}

  const char *what() const noexcept override
  {
    return message.c_str();
  }

  std::string message;
};


} // namespace TMX

#endif // COMMON_H
