#ifndef HORIZCONTROLSTATE_H
#define HORIZCONTROLSTATE_H

#include "actorstate.h"

namespace game {

class ActorControlComponent;

class HorizControlState : public ActorState
{
public:
  HorizControlState(ActorControlComponent &stateMachine, float acceleration, float friction, float maxSpeed = 320.f);

  virtual void update(std::uint32_t step, Game &game) override;

protected:
  // Rate at which speed increases when holding a direction
  float mAcceleration;
  // Rate at which speed decreases when not holding a direction
  float mFriction;
  float mMaxSpeed;
};

}

#endif // HORIZCONTROLSTATE_H
