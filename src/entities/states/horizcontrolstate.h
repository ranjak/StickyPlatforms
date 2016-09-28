#ifndef HORIZCONTROLSTATE_H
#define HORIZCONTROLSTATE_H

#include "herostate.h"

namespace game {


class HorizControlState : public HeroState
{
public:
  HorizControlState(Hero &hero);

  virtual void update(std::uint32_t step, GameState &game) override;

  virtual float getMaxSpeed();

  virtual float getAcceleration() = 0;
};

}

#endif // HORIZCONTROLSTATE_H
