#include "entityfactory.h"
#include "entity.h"
#include "rect.h"
#include "rectangle.h"
#include "movingphysicscomponent.h"
#include "staticphysicscomponent.h"
#include "gravitycomponent.h"
#include "basicaicomponent.h"
#include "playerinputcomponent.h"
#include "actorcontrolcomponent.h"
#include "log.h"
#include "make_unique.h"
#include "world/tile.h"
#include "color.h"
#include <string>

namespace game {

std::unique_ptr<Entity> EntityFactory::create(const std::string &type, const std::string &name, const Rect<float> &pos, EntityManager &manager, EntityID id)
{
  if (type == "PlayerStart")
    return std::unique_ptr<Entity>(new Entity(id, manager, pos.x, pos.y, 0, 0, name));

  else if (type == "Enemy") {

    std::unique_ptr<Entity> enemy(new Entity(id, manager, pos.x, pos.y, pos.w, pos.h, name, std::make_unique<Rectangle>(pos.w, pos.h, Color::RED)));

    std::unique_ptr<InputComponent> input = std::make_unique<BasicAiComponent>();
    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*enemy);
    std::unique_ptr<Component> control = std::make_unique<ActorControlComponent>(*enemy, *physics, *input, 75.f);

    enemy->addComponent(std::move(input));
    enemy->addComponent(std::move(control));
    enemy->addComponent(std::make_unique<GravityComponent>(*physics));
    enemy->addComponent(std::move(physics));

    return enemy;
  }

  else if (type == "Hero") {

    std::unique_ptr<Entity> hero(new Entity(id, manager, pos.x, pos.y, Tile::SIZE, Tile::SIZE, name, std::make_unique<Rectangle>(Tile::SIZE, Tile::SIZE, Color::GREEN)));

    std::unique_ptr<InputComponent> input = std::make_unique<PlayerInputComponent>();
    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*hero);
    std::unique_ptr<Component> control = std::make_unique<ActorControlComponent>(*hero, *physics, *input, 300.f);

    hero->addComponent(std::move(input));
    hero->addComponent(std::move(control));
    hero->addComponent(std::make_unique<GravityComponent>(*physics));
    hero->addComponent(std::move(physics));

    return hero;
  }

  else if (type == "invisibleWall") {
    std::unique_ptr<Entity> wall(new Entity(id, manager, pos.x, pos.y, pos.w, pos.h, name));

    wall->addComponent(std::make_unique<StaticPhysicsComponent>(*wall));

    return wall;
  }

  else {
    Log::getGlobal().get(Log::WARNING) << "Unknown entity type: "<<type<<" for entity "<<name<<std::endl;
    return nullptr;
  }
}


} // namespace game
