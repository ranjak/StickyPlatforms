#include "entitymanager.h"
#include "entityfactory.h"
#include "game.h"
#include "rect.h"
#include "log.h"
#include "graphicscomponent.h"
#include <algorithm>

namespace game {

using EntityPair = std::map<EntityID, std::unique_ptr<Entity>>::value_type;

EntityManager::EntityManager(Level &level) :
  mNextId(0),
  mPhysics(level),
  mEntities(),
  mLevel(level)
{

}

EntityManager::~EntityManager()
{
  /* Entities might access the entity map in their destructor.
   * This breaks if they try to access it while the map is being destroyed;
   * as a workaround, destroy them before destroying the map.
   * TODO entities shouldn't access the map in their dtor.
   */
  for (EntityPair &entity : mEntities) {
    entity.second.reset();
  }
}

EntityID EntityManager::makeEntity(const std::string &type, const std::string &name, const Rect<float> &pos, EntityID parent, const TMX::PropertyMap &properties)
{
  return EntityFactory::create(type, name, pos, *this, parent, properties);
}

Entity *EntityManager::makeEntity(const Rect<float> &pos, const std::string &name, EntityGroup group, EntityID parent)
{
  std::unique_ptr<Entity> entity(new Entity(mNextId, *this, pos, name, group, parent));

  if (entity) {
    Entity *ret = entity.get();
    mEntities.insert(std::make_pair(mNextId++, std::move(entity)));
    return ret;
  }
  else {
    game::error("EntityManager: Failed to create entity");
    return nullptr;
  }
}

EntityID EntityManager::createRemoteEntity(const std::string & type, const std::string & name, const Vector<float>& pos, const Vector<float> &vel, const Color & color, int hp)
{
  return EntityFactory::createRemoteEntity(type, name, pos, vel, *this, color, hp);
}

EntityID EntityManager::createLocalHero(const std::string & name, const Vector<float>& pos)
{
  return EntityFactory::createLocalHero(name, pos, *this);
}

Entity *EntityManager::getEntity(EntityID id) const
{
  ConstIterator it = mEntities.find(id);

  return (it != mEntities.end()) ? it->second.get() : nullptr;
}

Entity *EntityManager::getEntity(const std::string &name) const
{
  for (ConstIterator it=mEntities.begin(); it != mEntities.end(); it++) {
    if (it->second->getName() == name)
      return it->second.get();
  }

  return nullptr;
}

void EntityManager::update(std::uint32_t step, Game &game)
{
  for (EntityPair &entity : mEntities) {
    if (entity.second->isEnabled())
      entity.second->update(step, game);
  }

  for (Iterator it=mEntities.begin(); it != mEntities.end();) {

    if (it->second->isDead())
      it = mEntities.erase(it);
    else
      it++;
  }
}

void EntityManager::draw(Display &display, const Game &game)
{
  GraphicsComponent *graphics = nullptr;
//  const Camera &cam = game.getCamera();
//  const Rect<float> &viewport = cam.getViewport();

  for (const EntityPair &entity : mEntities) {
    if (entity.second->isEnabled() && (graphics = entity.second->getComponent<GraphicsComponent>())/*&& entity.second->getGlobalBox().intersects(viewport)*/)
      graphics->draw(display, entity.second->getGlobalPos());
  }
}

} // namespace game
