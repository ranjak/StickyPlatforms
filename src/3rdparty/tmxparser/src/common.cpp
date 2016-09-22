#include "common.h"
#include <rapidxml/rapidxml.hpp>
#include <string>

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

}
