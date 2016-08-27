#include "gamestate.h"
#include "hero.h"

namespace game {

const std::uint32_t TIMESTEP = 10;

GameState::GameState() :
  bindings(),
  entities()
{
  addEntity(std::unique_ptr<Entity>(new Hero));
}


void GameState::update(uint32_t step, const InputHandler *input)
{
  inputSnapshot = input;

  for (std::unique_ptr<Entity>& entity : entities) {
    entity->update(step, *this);
  }
}

void GameState::drawEntities(Display &target) const
{
  for (const std::unique_ptr<Entity>& entity : entities) {
    entity->draw(target);
  }
}

bool GameState::isCommandPressed(Command cmd)
{
  return inputSnapshot->isKeyPressed(bindings.getBinding(cmd));
}

void GameState::addEntity(std::unique_ptr<Entity>&& entity)
{
  entities.push_back(std::move(entity));
}


} //namespace game
