#include "tmxcommon.h"

namespace TMX {

Property::Property(Property::valType type, const std::string &value) :
  type(type)
{
  switch (type) {
  case STRING:
    strVal = value;
    break;
  case INT:
    intVal = std::stoi(value);
    break;
  case FLOAT:
    floatVal = std::stof(value);
    break;
  case BOOL:
    boolVal = (value == "true") ? true : false;
    break;
  }
}

Property::~Property()
{
  if (type == STRING)
    strVal.std::string::~string();
}

Property::Property(const Property &p)
{
  this->operator =(p);
}

Property &Property::operator=(const Property &p)
{
  switch (p.type) {
  case STRING:
    strVal = p.strVal;
    break;
  case INT:
    intVal = p.intVal;
    break;
  case FLOAT:
    floatVal = p.floatVal;
    break;
  case BOOL:
    boolVal = p.boolVal;
    break;
  }

  return *this;
}

Property::Property(Property &&p)
{
  this->operator =(std::move(p));
}

Property &Property::operator=(Property &&p)
{
  switch (p.type) {
  case STRING:
    strVal = std::move(p.strVal);
    break;
  case INT:
    intVal = p.intVal;
    break;
  case FLOAT:
    floatVal = p.floatVal;
    break;
  case BOOL:
    boolVal = p.boolVal;
    break;
  }

  return *this;
}

}
