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
  GroundState(Hero& hero);

  void update(std::uint32_t step, GameState &game) override;

  float getAcceleration() override;

};

}

#endif // GROUNDSTATE_H
