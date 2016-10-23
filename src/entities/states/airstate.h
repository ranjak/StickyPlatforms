#ifndef AIRSTATE_H
#define AIRSTATE_H

#include "horizcontrolstate.h"

namespace game {

struct Message;

class AirState : public HorizControlState
{
public:
  AirState(ActorControlComponent &stateMachine, float maxSpeed, float maxAirSpeed);

  void enter() override;

  void update(std::uint32_t step, GameState &game) override;

  void receiveMessage(Message &msg) override;

private:
  void updateWallHug(std::uint32_t step, GameState &game);

private:
  float mMaxAirSpeed;
  // If we're in contact with a wall, normal of the contact
  int mWallHugNormal;
  // When hugging a wall, duration in milliseconds during which a walljump can be performed
  // while holding the direction opposite to the wall
  int mWalljumpTimeFrame;
};

}

#endif // AIRSTATE_H
