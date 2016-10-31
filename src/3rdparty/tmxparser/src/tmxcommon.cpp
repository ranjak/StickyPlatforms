#include "tmxcommon.h"

namespace TMX {

Property::Property(Property::valType type, const std::string &value) :
  type(type)
{
  switch (type) {
  case STRING:
    new (&strVal) std::string(value);
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

Property::Property(const Property &p) :
  type(p.type)
{
  switch (type) {
  case STRING:
    new (&strVal) std::string(p.strVal);
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
}

Property &Property::operator=(const Property &p)
{
  if (&p == this)
    return *this;

  if (type == STRING && p.type != STRING)
    strVal.std::string::~string();

  switch (p.type) {
  case STRING:
    if (type == STRING)
      strVal = p.strVal;
    else
      new (&strVal) std::string(p.strVal);
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

  type = p.type;

  return *this;
}

Property::Property(Property &&p) :
  type(p.type)
{
  switch (type) {
  case STRING:
    new (&strVal) std::string(std::move(p.strVal));
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
}

Property &Property::operator=(Property &&p)
{
  if (&p == this)
    return *this;

  if (type == STRING && p.type != STRING)
    strVal.std::string::~string();

  switch (p.type) {
  case STRING:
    if (type == STRING)
      strVal = std::move(p.strVal);
    else
      new (&strVal) std::string(std::move(p.strVal));
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

  type = p.type;

  return *this;
}

const Property* PropertyMap::find(const std::string &name, Property::valType type) const
{
  auto prop = properties.find(name);

  if (prop == properties.end() || prop->second.type != type)
    return nullptr;

  return &prop->second;
}

bool PropertyMap::getBool(const std::string &name, bool &value, bool defaultVal) const
{
  const Property *p = find(name, Property::BOOL);

  value = p ? p->boolVal : defaultVal;
  return (p);
}

bool PropertyMap::getFloat(const std::string &name, float &value, float defaultVal) const
{
  const Property *p = find(name, Property::FLOAT);

  value = p ? p->floatVal : defaultVal;
  return (p);
}

bool PropertyMap::getInt(const std::string &name, int &value, int defaultVal) const
{
  const Property *p = find(name, Property::INT);

  value = p ? p->intVal : defaultVal;
  return (p);
}

bool PropertyMap::getString(const std::string &name, std::string &value, const std::string &defaultVal) const
{
  const Property *p = find(name, Property::STRING);

  value = p ? p->strVal : defaultVal;
  return (p);
}

}
