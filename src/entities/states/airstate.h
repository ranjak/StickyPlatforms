#ifndef AIRSTATE_H
#define AIRSTATE_H

#include "horizcontrolstate.h"

namespace game {

struct Message;

class AirState : public HorizControlState
{
public:
  AirState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed);

  void update(std::uint32_t step, GameState &game) override;

  void receiveMessage(Message &msg) override;

private:
  float mMaxAirSpeed;
};

}

#endif // AIRSTATE_H
