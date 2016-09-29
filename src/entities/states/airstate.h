#ifndef AIRSTATE_H
#define AIRSTATE_H

#include "horizcontrolstate.h"

namespace game {


class AirState : public HorizControlState
{
public:
  AirState(Hero& hero);

  void update(std::uint32_t step, GameState &game) override;

  float getAcceleration() override;
};

}

#endif // AIRSTATE_H
