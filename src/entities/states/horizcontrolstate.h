#ifndef HORIZCONTROLSTATE_H
#define HORIZCONTROLSTATE_H

#include "herostate.h"

namespace game {

class WalkComponent;

class HorizControlState : public HeroState
{
public:
  HorizControlState(PlayerInputComponent &stateMachine, WalkComponent &walkComp, float acceleration, float maxSpeed = 320.f);

  virtual void update(std::uint32_t step, GameState &game) override;

  void enter() override;

protected:
  WalkComponent &mWalkComp;
private:
  int mDirection;
  float mAcceleration;
  float mMaxSpeed;
};

}

#endif // HORIZCONTROLSTATE_H
