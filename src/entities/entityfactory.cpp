#include "entityfactory.h"
#include "entity.h"
#include "enemy.h"
#include "rect.h"
#include <string>

namespace game {

std::unique_ptr<Entity> EntityFactory::create(const std::string &type, const std::string &name, const Rect<int> &pos)
{
  if (type == "PlayerStart")
    return std::unique_ptr<Entity>(new Entity(pos.x, pos.y, 0, 0, false, name));

  else if (type == "Enemy")
    return std::unique_ptr<Entity>(new Enemy(pos.x, pos.y, pos.w, pos.h, name));

  else
    return nullptr;
}


} // namespace game
