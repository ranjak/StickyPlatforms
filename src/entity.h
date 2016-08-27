#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include "gamestate.h"

namespace game {

class GameState;

class Entity
{
public:
  Entity();

  void update(std::uint32_t step, GameState& game);
};

} // namespace game

#endif // ENTITY_H
