#include "entityfactory.h"
#include "entity.h"
#include "rect.h"
#include "rectangle.h"
#include "movingphysicscomponent.h"
#include "staticphysicscomponent.h"
#include "basicaicomponent.h"
#include "playerinputcomponent.h"
#include "actorcontrolcomponent.h"
#include "swordcomponent.h"
#include "weaponcomponent.h"
#include "damagecomponent.h"
#include "healthcomponent.h"
#include "victorytrigger.h"
#include "log.h"
#include "make_unique.h"
#include "world/tile.h"
#include "color.h"
#include "entitymanager.h"
#include "entitygroup.h"
#include "text.h"
#include "tmxcommon.h"
#include "gamestate.h"
#include <string>

namespace game {

EntityID EntityFactory::create(const std::string &type, const std::string &name, const Rect<float> &pos, EntityManager &manager, EntityID id, EntityID parent, const std::map<std::string, TMX::Property> &properties)
{
  if (type == "PlayerStart")
    return manager.makeEntity(pos, name)->id;

  else if (type == "Enemy") {

    Entity *enemy = manager.makeEntity(pos, name, EntityGroup::ENEMY, std::make_unique<Rectangle>(pos.w, pos.h, Color::RED), parent);

    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*enemy, false, true);
    std::unique_ptr<InputComponent> input = std::make_unique<BasicAiComponent>(*physics);
    std::unique_ptr<Component> control = std::make_unique<ActorControlComponent>(*enemy, *physics, *input, 75.f, 75.f);

    enemy->addComponent(std::move(input));
    enemy->addComponent(std::move(control));
    enemy->addComponent(std::move(physics));
    enemy->addComponent(std::make_unique<HealthComponent>(*enemy, 3));
    enemy->addComponent(std::make_unique<DamageComponent>(1, EntityGroup::ALLY));

    return enemy->id;
  }

  else if (type == "Hero") {

    Entity *hero = manager.makeEntity(pos, name, EntityGroup::ALLY, std::make_unique<Rectangle>(Tile::SIZE, Tile::SIZE, Color::GREEN), parent);

    std::unique_ptr<InputComponent> input = std::make_unique<PlayerInputComponent>();
    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*hero, false, true);
    std::unique_ptr<Component> control = std::make_unique<ActorControlComponent>(*hero, *physics, *input, 300.f, 200.f);

    hero->addComponent(std::move(input));
    hero->addComponent(std::move(control));
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

    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*sword, false, false);
    physics->setIgnoresObstacles(true);

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

  else if (type == "VictoryTrigger") {

    Entity *victory = manager.makeEntity(pos, name);

    // Get the next level filename if any
    std::string nextLevel = "";
    auto nextLevelProp = properties.find("nextLevel");

    if (nextLevelProp != properties.end() && nextLevelProp->second.type == TMX::Property::STRING)
      nextLevel = nextLevelProp->second.strVal;

    victory->addComponent(std::make_unique<StaticPhysicsComponent>(*victory, false));
    victory->addComponent(std::make_unique<VictoryTrigger>(nextLevel));

    return victory->id;
  }

  else if (type == "TextLine") {

    auto content = properties.find("text");
    std::string textContent = "";
    if (content != properties.end() && content->second.type == TMX::Property::STRING)
      textContent = content->second.strVal;
    else
      Log::getGlobal().get(Log::WARNING) << "EntityFactory: TextLine (id="<<id<<") has no \"text\" property"<<std::endl;

    std::unique_ptr<Text> text = std::make_unique<Text>(GameState::current().getDisplay(), textContent, static_cast<int>(pos.h));

    return manager.makeEntity(pos, name, EntityGroup::NONE, std::move(text))->id;
  }

  else {
    Log::getGlobal().get(Log::WARNING) << "EntityFactory: Unknown entity type: "<<type<<" for entity "<<name<<std::endl;
    return Entity::none;
  }
}


} // namespace game
