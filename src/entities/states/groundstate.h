#ifndef GROUNDSTATE_H
#define GROUNDSTATE_H

#include "horizcontrolstate.h"

namespace game {


/**
 * @brief The state the hero is in when it's on the ground.
 */
class GroundState : public HorizControlState
{
public:
  GroundState(PlayerInputComponent &stateMachine, WalkComponent &walkComp);

  void update(std::uint32_t step, GameState &game) override;
};

}

#endif // GROUNDSTATE_H
