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
#include "swordcomponent.h"
#include "weaponcomponent.h"
#include "damagecomponent.h"
#include "healthcomponent.h"
#include "log.h"
#include "make_unique.h"
#include "world/tile.h"
#include "color.h"
#include "entitymanager.h"
#include "entitygroup.h"
#include <string>

namespace game {

EntityID EntityFactory::create(const std::string &type, const std::string &name, const Rect<float> &pos, EntityManager &manager, EntityID id, EntityID parent)
{
  if (type == "PlayerStart")
    return manager.makeEntity(pos, name)->id;

  else if (type == "Enemy") {

    Entity *enemy = manager.makeEntity(pos, name, EntityGroup::ENEMY, std::make_unique<Rectangle>(pos.w, pos.h, Color::RED), parent);

    std::unique_ptr<InputComponent> input = std::make_unique<BasicAiComponent>();
    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*enemy);
    std::unique_ptr<Component> control = std::make_unique<ActorControlComponent>(*enemy, *physics, *input, 75.f, 75.f);

    enemy->addComponent(std::move(input));
    enemy->addComponent(std::move(control));
    enemy->addComponent(std::make_unique<GravityComponent>(*physics));
    enemy->addComponent(std::move(physics));
    enemy->addComponent(std::make_unique<HealthComponent>(*enemy, 3));
    enemy->addComponent(std::make_unique<DamageComponent>(1, EntityGroup::ALLY));

    return enemy->id;
  }

  else if (type == "Hero") {

    Entity *hero = manager.makeEntity(pos, name, EntityGroup::ALLY, std::make_unique<Rectangle>(Tile::SIZE, Tile::SIZE, Color::GREEN), parent);

    std::unique_ptr<InputComponent> input = std::make_unique<PlayerInputComponent>();
    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*hero);
    std::unique_ptr<Component> control = std::make_unique<ActorControlComponent>(*hero, *physics, *input, 300.f, 200.f);

    hero->addComponent(std::move(input));
    hero->addComponent(std::move(control));
    hero->addComponent(std::make_unique<GravityComponent>(*physics));
    hero->addComponent(std::move(physics));
    hero->addComponent(std::make_unique<WeaponComponent>(*hero));
    hero->addComponent(std::make_unique<HealthComponent>(*hero, 5));

    return hero->id;
  }

  else if (type == "sword") {

    if (parent == Entity::none) {
      game::error("EntityFactory: couldn't create sword (id="+std::to_string(id)+",name="+name+"): no parent specified");
      return Entity::none;
    }
    Entity *sword = manager.makeEntity(pos, name, EntityGroup::NONE, std::make_unique<Rectangle>(pos.w, pos.h, Color::BLUE), parent);

    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*sword, false);

    sword->addComponent(std::make_unique<SwordComponent>(*physics));
    sword->addComponent(std::move(physics));
    sword->addComponent(std::make_unique<DamageComponent>(1, EntityGroup::ENEMY));

    return sword->id;
  }

  else if (type == "invisibleWall") {

    Entity *wall = manager.makeEntity(pos, name);

    wall->addComponent(std::make_unique<StaticPhysicsComponent>(*wall, true));

    return wall->id;
  }

  else {
    Log::getGlobal().get(Log::WARNING) << "Unknown entity type: "<<type<<" for entity "<<name<<std::endl;
    return Entity::none;
  }
}


} // namespace game
