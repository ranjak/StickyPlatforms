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
  Entity(int x, int y);

  virtual void update(std::uint32_t step, GameState& game) = 0;

  virtual void draw(Display& target) const = 0;

  Vector<int> getPosition() const;

  void setPosition(const Vector<int>& newPos);

protected:
  Vector<int> pos;
};

} // namespace game

#endif // ENTITY_H
