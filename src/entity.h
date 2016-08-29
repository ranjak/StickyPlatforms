#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include "gamestate.h"
#include "gamevector.h"
#include "display.h"
#include "rect.h"

namespace game {

class GameState;
class Display;

class Entity
{
public:
  Entity();
  Entity(int x, int y, int w=0, int h=0);

  virtual void update(std::uint32_t step, GameState& game) = 0;

  virtual void draw(Display& target) const = 0;

  Vector<int> getPosition() const;

  void setPosition(const Vector<int>& newPos);

  const Rect& getBoundingBox();

protected:
  Rect boundingBox;
};

} // namespace game

#endif // ENTITY_H
