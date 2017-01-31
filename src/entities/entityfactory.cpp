#include "entityfactory.h"
#include "entity.h"
#include "rect.h"
#include "rectangle.h"
#include "herosquare.h"
#include "movingphysicscomponent.h"
#include "staticphysicscomponent.h"
#include "basicaicomponent.h"
#include "playerinputcomponent.h"
#include "actorcontrolcomponent.h"
#include "swordcomponent.h"
#include "weaponcomponent.h"
#include "damagecomponent.h"
#include "healthcomponent.h"
#include "triggercomponent.h"
#include "triggertypes.h"
#include "graphicscomponent.h"
#include "cameracomponent.h"
#include "cameracontroller.h"
#include "log.h"
#include "make_unique.h"
#include "world/tile.h"
#include "color.h"
#include "entitymanager.h"
#include "entitygroup.h"
#include "text.h"
#include "tmxcommon.h"
#include "game.h"
#include "util.h"
#include "stormancerconnection.h"
#include <string>

namespace game {

EntityID EntityFactory::create(const std::string &type, const std::string &name, const Rect<float> &pos, EntityManager &manager, EntityID parent, const TMX::PropertyMap &properties)
{
  if (type == "PlayerStart")
    return manager.makeEntity(pos, name)->id;

  else if (type == "Enemy") {

    Entity *enemy = manager.makeEntity(pos, name, EntityGroup::ENEMY, parent);

    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*enemy, false, true);
    std::unique_ptr<InputComponent> input = std::make_unique<BasicAiComponent>(*physics);
    std::unique_ptr<Component> control = std::make_unique<ActorControlComponent>(*enemy, *physics, *input, 75.f, 75.f);

    enemy->addComponent(std::move(input));
    enemy->addComponent(std::move(control));
    enemy->addComponent(std::move(physics));
    enemy->addComponent(std::make_unique<HealthComponent>(*enemy, 3));
    enemy->addComponent(std::make_unique<DamageComponent>(1, EntityGroup::ALLY, 0.5f));
    enemy->addComponent(std::make_unique<GraphicsComponent>(std::make_unique<Rectangle>(pos.w, pos.h, Color::RED)));

    return enemy->id;
  }

  else if (type == "Hero") {

    Entity *hero = manager.makeEntity(pos, name, EntityGroup::ALLY, parent);

    Color color(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));

    std::unique_ptr<InputComponent> input = std::make_unique<PlayerInputComponent>();
    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*hero, false, true);
    std::unique_ptr<ActorControlComponent> control = std::make_unique<ActorControlComponent>(*hero, *physics, *input, 300.f, 200.f);
    std::unique_ptr<GraphicsComponent> graphics = std::make_unique<GraphicsComponent>(std::make_unique<HeroSquare>(pos.w, pos.h, color), control.get());

    hero->addComponent(std::move(input));
    hero->addComponent(std::move(control));
    hero->addComponent(std::move(physics));
    hero->addComponent(std::make_unique<WeaponComponent>(*hero));
    hero->addComponent(std::make_unique<HealthComponent>(*hero, 5));
    hero->addComponent(std::move(graphics));
    hero->addComponent(std::make_unique<CameraComponent>(*hero, std::make_unique<CameraAnchoredY>(*hero)));

    Game::current().network().spawn(color, Vector<float>(pos.x, pos.y), 5);

    return hero->id;
  }

  else if (type == "sword") {

    if (parent == Entity::none) {
      game::error("EntityFactory: couldn't create sword (name="+name+"): no parent specified");
      return Entity::none;
    }
    Entity *sword = manager.makeEntity(pos, name, EntityGroup::NONE, parent);

    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*sword, false, false);
    physics->setIgnoresObstacles(true);

    sword->addComponent(std::make_unique<SwordComponent>(*physics));
    sword->addComponent(std::move(physics));
    sword->addComponent(std::make_unique<DamageComponent>(1, EntityGroup::ENEMY, 1.f));
    sword->addComponent(std::make_unique<GraphicsComponent>(std::make_unique<Rectangle>(pos.w, pos.h, Color::BLUE)));

    return sword->id;
  }

  else if (type == "invisibleWall") {

    Entity *wall = manager.makeEntity(pos, name);

    wall->addComponent(std::make_unique<StaticPhysicsComponent>(*wall, true));

    return wall->id;
  }

  else if (type == "Trigger") {

    Entity *trigger = manager.makeEntity(pos, name);

    std::unique_ptr<PhysicsComponent> physics = std::make_unique<StaticPhysicsComponent>(*trigger, false);

    std::string triggerType;
    bool requireOnGround;
    bool singleShot;

    (void) properties.getBool("requireOnGround", requireOnGround, false);
    (void) properties.getBool("isSingleShot", singleShot, false);

    if (properties.getString("type", triggerType))
      trigger->addComponent(std::make_unique<TriggerComponent>(makeTrigger(triggerType, properties, trigger->id), *physics, requireOnGround, singleShot));
    else
      Log::getGlobal().get(Log::WARNING) << "EntityFactory: Trigger "<<*trigger<<" has no \"type\" property"<<std::endl;

    trigger->addComponent(std::move(physics));
    return trigger->id;
  }

  else if (type == "TextLine") {

    std::string textContent = "";

    if (!properties.getString("text", textContent))
      Log::getGlobal().get(Log::WARNING) << "EntityFactory: TextLine (name="<<name<<") has no \"text\" property"<<std::endl;

    Entity* line = manager.makeEntity(pos, name, EntityGroup::NONE, parent);
    line->addComponent(std::make_unique<GraphicsComponent>(std::make_unique<Text>(Game::current().getDisplay(), textContent, static_cast<int>(pos.h))));

    return line->id;
  }

  else {
    Log::getGlobal().get(Log::WARNING) << "EntityFactory: Unknown entity type: \""<<type<<"\" for entity \""<<name<<"\""<<std::endl;
    return Entity::none;
  }
}

EntityID EntityFactory::createRemoteEntity(const std::string &type, const std::string &name, const Vector<float> &pos, const Vector<float> &vel, EntityManager &manager, const Color &color, int hp)
{
  if (type == "RemoteHero") {

    Entity *remoteHero = manager.makeEntity(Rect<float>(pos.x, pos.y, 32, 32), name, EntityGroup::ALLY);
    remoteHero->setRemote(true);

    std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*remoteHero, false, true, vel);
    std::unique_ptr<InputComponent> input = std::make_unique<BasicAiComponent>(*physics);
    std::unique_ptr<ActorControlComponent> control = std::make_unique<ActorControlComponent>(*remoteHero, *physics, *input, 300.f, 200.f);
    std::unique_ptr<GraphicsComponent> graphics = std::make_unique<GraphicsComponent>(std::make_unique<HeroSquare>(32, 32, color), control.get());

    remoteHero->addComponent(std::move(input));
    remoteHero->addComponent(std::move(control));
    remoteHero->addComponent(std::move(physics));
    remoteHero->addComponent(std::make_unique<WeaponComponent>(*remoteHero));
    remoteHero->addComponent(std::make_unique<HealthComponent>(*remoteHero, hp, 5));
    remoteHero->addComponent(std::move(graphics));

    return remoteHero->id;
  }
  else {
    Log::getGlobal().get(Log::WARNING) << "EntityFactory: Unknown entity type: \"" << type << "\" for entity \"" << name << "\"" << std::endl;
    return Entity::none;
  }
}

EntityID EntityFactory::createLocalHero(const std::string &name, const Vector<float> &pos, EntityManager &manager)
{
  static Color color(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
  
  Entity *hero = manager.makeEntity(Rect<float>(pos.x, pos.y, 32, 32), name, EntityGroup::ALLY);

  std::unique_ptr<InputComponent> input = std::make_unique<PlayerInputComponent>();
  std::unique_ptr<MovingPhysicsComponent> physics = std::make_unique<MovingPhysicsComponent>(*hero, false, true);
  std::unique_ptr<ActorControlComponent> control = std::make_unique<ActorControlComponent>(*hero, *physics, *input, 300.f, 200.f);
  std::unique_ptr<GraphicsComponent> graphics = std::make_unique<GraphicsComponent>(std::make_unique<HeroSquare>(32, 32, color), control.get());

  hero->addComponent(std::move(input));
  hero->addComponent(std::move(control));
  hero->addComponent(std::move(physics));
  hero->addComponent(std::make_unique<WeaponComponent>(*hero));
  hero->addComponent(std::make_unique<HealthComponent>(*hero, 5));
  hero->addComponent(std::move(graphics));
  hero->addComponent(std::make_unique<CameraComponent>(*hero, std::make_unique<CameraAnchoredY>(*hero)));

  Game::current().network().spawn(color, Vector<float>(pos.x, pos.y), 5);

  return hero->id;
}


} // namespace game
