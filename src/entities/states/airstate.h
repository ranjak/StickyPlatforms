#ifndef AIRSTATE_H
#define AIRSTATE_H

#include "horizcontrolstate.h"

namespace game {


class AirState : public HorizControlState
{
public:
  AirState(Hero& hero);

  void update(std::uint32_t step, GameState &game) override;

  void enter() override;

  float getAcceleration() override;

private:
  static const float GRAVITY;
  static const float FALL_SPEED;
};

}

#endif // AIRSTATE_H
