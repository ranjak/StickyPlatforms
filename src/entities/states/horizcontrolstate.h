#ifndef HORIZCONTROLSTATE_H
#define HORIZCONTROLSTATE_H

#include "actorstate.h"

namespace game {

class ActorControlComponent;

class HorizControlState : public ActorState
{
public:
  HorizControlState(ActorControlComponent &stateMachine, float acceleration, float maxSpeed = 320.f);

  virtual void update(std::uint32_t step, GameState &game) override;

  int getDirection() override;

private:
  int mDirection;
  float mAcceleration;
  float mMaxSpeed;
};

}

#endif // HORIZCONTROLSTATE_H
