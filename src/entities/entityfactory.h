#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <memory>
#include <iosfwd>

namespace game {

class Entity;
template<typename T> class Rect;

class EntityFactory
{
public:

  static std::unique_ptr<Entity> create(const std::string &type, const std::string &name, const Rect<int> &pos);
};

} // namespace game

#endif // ENTITYFACTORY_H
