#ifndef CLINGSTATE_H
#define CLINGSTATE_H

#include "horizcontrolstate.h"

namespace game {

class ClingState : public HorizControlState
{
public:
  ClingState(ActorControlComponent &stateMachine);

  void enter();

  void update(uint32_t step, Game &game) override;

  void exit() override;

private:
  bool canStillCling(Game &game);

private:
  // You can only cling for a few seconds
  std::uint32_t mClingTimeout;
};

} // namespace game

#endif // CLINGSTATE_H
