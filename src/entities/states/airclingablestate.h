#ifndef AIRCLINGABLESTATE_H
#define AIRCLINGABLESTATE_H

#include "airstate.h"

namespace game {

class AirClingableState : public AirState
{
public:
  AirClingableState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed, float friction);

  void receiveMessage(Message &msg) override;
};

} // namespace game

#endif // AIRCLINGABLESTATE_H
