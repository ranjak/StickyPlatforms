#include "entitymanager.h"
#include "entityfactory.h"
#include "gamestate.h"
#include "rect.h"
#include <algorithm>

namespace game {

using EntityPair = std::map<EntityID, std::unique_ptr<Entity>>::value_type;

EntityManager::EntityManager() :
  mNextId(0),
  mEntities()
{

}

EntityID EntityManager::makeEntity(const std::string &type, const std::string &name, const Rect<float> &pos)
{
  std::unique_ptr<Entity> entity = EntityFactory::create(type, name, pos, *this, mNextId);

  if (entity) {
    mEntities.insert(std::make_pair(mNextId, std::move(entity)));
    return mNextId++;
  }
  else
    return Entity::none;
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

void EntityManager::update(std::uint32_t step, GameState &game)
{
  for (Iterator it=mEntities.begin(); it != mEntities.end();) {
    it->second->update(step, game);

    if (it->second->isDead())
      it = mEntities.erase(it);
    else
      it++;
  }
}

void EntityManager::draw(Display &display, const GameState &game) const
{
  const Camera &cam = game.getCamera();
  const Rect<float> &viewport = cam.getViewport();

  for (const EntityPair &entity : mEntities) {
    if (entity.second->getGlobalBox().intersects(viewport))
      entity.second->draw(display, cam);
  }
}

void EntityManager::handleCollisions(Entity &entity)
{
  const Rect<float> &box = entity.getGlobalBox();

  for (const EntityPair &other : mEntities) {
    if (other.first != entity.id && other.second->isCollidable() && box.touches(other.second->getGlobalBox())) {
      other.second->onCollision(entity);
      entity.onCollision(*other.second);
    }
  }
}

bool EntityManager::isStandingOnEntity(Entity &entity) const
{
  const Rect<float> &box = entity.getGlobalBox();

  for (const EntityPair &other : mEntities) {

    const Rect<float> &obox = other.second->getGlobalBox();

    if (other.first != entity.id && other.second->isObstacle() && (box.y + box.h == obox.y) && (box.x + box.w > obox.x) && (box.x < obox.x + obox.w))
      return true;
  }

  return false;
}

} // namespace game
