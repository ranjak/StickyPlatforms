#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "entity.h"
#include <memory>
#include <string>
#include <map>

namespace TMX {
struct PropertyMap;
}

namespace game {

template<typename T> class Rect;
class EntityManager;

class EntityFactory
{
  friend class EntityManager;
private:

  static EntityID create(const std::string &type, const std::string &name, const Rect<float> &pos, EntityManager &manager, EntityID parent, const TMX::PropertyMap &properties);
};

} // namespace game

#endif // ENTITYFACTORY_H
