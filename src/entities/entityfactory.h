#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <memory>
#include <iosfwd>
#include "entity.h"

namespace game {

template<typename T> class Rect;
class EntityManager;

class EntityFactory
{
  friend class EntityManager;
private:

  static EntityID create(const std::string &type, const std::string &name, const Rect<float> &pos, EntityManager &manager, EntityID id, EntityID parent);
};

} // namespace game

#endif // ENTITYFACTORY_H
