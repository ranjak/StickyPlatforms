#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity.h"
#include "entitygroup.h"
#include "physicsmanager.h"
#include "tmxcommon.h"
#include <map>
#include <memory>
#include <cstdint>
#include <vector>
#include <string>


namespace game {

template<typename T> class Rect;
class Game;
class Display;
class Level;
class Graphics;

class EntityManager
{
public:
  using Iterator = std::map<EntityID, std::unique_ptr<Entity>>::iterator;
  using ConstIterator = std::map<EntityID, std::unique_ptr<Entity>>::const_iterator;

  EntityManager(Level &level);

  EntityID makeEntity(const std::string &type, const std::string &name, const Rect<float> &pos, EntityID parent=Entity::none, const TMX::PropertyMap &properties={});
  Entity *makeEntity(const Rect<float> &pos, const std::string &name="", EntityGroup group=EntityGroup::NONE, EntityID parent=Entity::none);

  Entity *getEntity(EntityID id) const;
  Entity *getEntity(const std::string &name) const;

  void update(std::uint32_t step, Game &game);

  void draw(Display &display, const Game &game);

  Level &getLevel() { return mLevel; }

  PhysicsManager &getPhysics() { return mPhysics; }

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
  PhysicsManager mPhysics;
  std::map<EntityID, std::unique_ptr<Entity>> mEntities;
  Level &mLevel;
};

} // namespace game

#endif // ENTITYMANAGER_H
