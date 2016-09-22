#ifndef COMMON_H
#define COMMON_H

#include <rapidxml/rapidxml.hpp>
#include <string>


namespace TMX {


std::string getDirectory(std::string filename);

const char* findOrFail(rapidxml::xml_node<>* node, const char* attribute);

const char* presentOrDefault(rapidxml::xml_attribute<>* attribute, const char* defaultVal);


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
