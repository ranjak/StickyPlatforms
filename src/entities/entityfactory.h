#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "entity.h"
#include "tmxcommon.h"
#include <memory>
#include <string>
#include <map>

namespace game {

template<typename T> class Rect;
class EntityManager;

class EntityFactory
{
  friend class EntityManager;
private:

  static EntityID create(const std::string &type, const std::string &name, const Rect<float> &pos, EntityManager &manager, EntityID id, EntityID parent, const std::map<std::string, TMX::Property> &properties);
};

} // namespace game

#endif // ENTITYFACTORY_H
