#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include "gamestate.h"
#include "gamevector.h"
#include "display.h"

namespace game {

class GameState;
class Display;

class Entity
{
public:
  Entity();

  virtual void update(std::uint32_t step, GameState& game) = 0;

  virtual void draw(Display& target) const = 0;

protected:
  Vector<int> pos;
};

} // namespace game

#endif // ENTITY_H
