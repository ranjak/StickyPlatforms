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

TMX::Image parseImage(rapidxml::xml_node<>* img_node, const std::string &currentDir)
{
  TMX::Image image = {};

  image.source = currentDir + TMX::findOrFail( img_node, "source" );
  image.width = std::atoi( TMX::presentOrDefault( img_node->first_attribute( "width" ), "0" ) );
  image.height = std::atoi( TMX::presentOrDefault( img_node->first_attribute( "height" ), "0" ) );

  if( img_node->first_attribute( "trans" ) != 0 ) {
    image.transparentColor = std::atoi( img_node->first_attribute( "trans" )->value() );
  }

  return image;
}

Object parseObject(rapidxml::xml_node<> *obj_node)
{
  TMX::Object object = {};

  return object;
}

}
