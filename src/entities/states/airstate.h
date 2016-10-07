#ifndef AIRSTATE_H
#define AIRSTATE_H

#include "horizcontrolstate.h"

namespace game {


class AirState : public HorizControlState
{
public:
  AirState(ActorControlComponent &stateMachine);

  void update(std::uint32_t step, GameState &game) override;
};

}

#endif // AIRSTATE_H
