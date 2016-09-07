#include "gamestate.h"
#include "hero.h"
#include "platform.h"
#include <algorithm>

namespace game {

const std::uint32_t TIMESTEP = 10;

GameState::GameState() :
  mBindings(),
  mEntities()
{
  addEntity(std::unique_ptr<Entity>(new Hero));
  addEntity(std::unique_ptr<Entity>(new Platform(150, 150)));
}


void GameState::update(uint32_t step, const InputHandler *input)
{
  mInputSnapshot = input;

  for (std::unique_ptr<Entity>& entity : mEntities) {
    entity->update(step, *this);
  }
}

void GameState::drawEntities(Display &target) const
{
  for (const std::unique_ptr<Entity>& entity : mEntities) {
    entity->draw(target);
  }
}

bool GameState::isCommandPressed(Command cmd)
{
  return mInputSnapshot->isKeyPressed(mBindings.getBinding(cmd));
}

void GameState::addEntity(std::unique_ptr<Entity>&& entity)
{
  mEntities.push_back(std::move(entity));
}

bool GameState::collides(const Entity &entity)
{
  // Check only the entities after the given entity in sequential order
  auto entityPos = std::find_if(mEntities.begin(), mEntities.end(), [&] (std::unique_ptr<Entity>& vecEntity) { return vecEntity.get() == &entity; });

  for (auto it = entityPos+1; it != mEntities.end(); it++) {
    if (entity.getBoundingBox().intersects((*it)->getBoundingBox()))
      return true;
  }

  return false;
}

std::vector<CollisionManifold> GameState::checkCollisions(Entity &entity)
{
  // Check only the entities after the given entity in sequential order
  auto entityPos = std::find_if(mEntities.begin(), mEntities.end(), [&] (std::unique_ptr<Entity>& vecEntity) { return vecEntity.get() == &entity; });

  std::vector<CollisionManifold> collisions;

  for (auto it = entityPos+1; it != mEntities.end(); it++) {
    if (entity.getBoundingBox().intersects((*it)->getBoundingBox()))
      collisions.push_back(CollisionManifold{**it, entity.getBoundingBox().getCollisionNormal((*it)->getBoundingBox())});
  }

  return collisions;
}


} //namespace game
