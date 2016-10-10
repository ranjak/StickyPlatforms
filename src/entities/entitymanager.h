#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity.h"
#include <map>
#include <memory>
#include <cstdint>
#include <vector>

namespace game {

template<typename T> class Rect;
class GameState;
class Display;

class EntityManager
{
public:
  using Iterator = std::map<EntityID, std::unique_ptr<Entity>>::iterator;
  using ConstIterator = std::map<EntityID, std::unique_ptr<Entity>>::const_iterator;

  EntityManager();

  EntityID makeEntity(const std::string &type, const std::string &name, const Rect<float> &pos);

  Entity *getEntity(EntityID id) const;
  Entity *getEntity(const std::string &name) const;

  void update(std::uint32_t step, GameState &game);

  void draw(Display &display, const GameState &game) const;

  void handleCollisions(Entity &entity);

  bool isStandingOnEntity(Entity &entity) const;

  /**
   * @brief getEntitiesInArea Get every entity which bounding box intersects with \p area.
   * @param area Rectangular area to scan for entities.
   * @param pred Optional predicate to filter the entities. Can take an entity reference as parameter.
   * @return Vector containing pointers to the entities that overlap \p area.
   * You do not own these pointers.
   */
  template<typename Func>
  std::vector<Entity*> getEntitiesInArea(const Rect<float> &area, Func &&pred=[](){return true;}) const
  {
    std::vector<Entity*> entities;

    for (ConstIterator it=mEntities.begin(); it != mEntities.end(); it++) {
      if (pred(*(it->second)) && area.intersects(it->second->getGlobalBox()))
        entities.push_back(it->second.get());
    }

    return entities;
  }

private:
  EntityID mNextId;
  std::map<EntityID, std::unique_ptr<Entity>> mEntities;
};

} // namespace game

#endif // ENTITYMANAGER_H
