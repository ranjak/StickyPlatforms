#ifndef AUTOINPUTCOMPONENT_H
#define AUTOINPUTCOMPONENT_H

#include "inputcomponent.h"
#include <vector>

namespace game {

enum Command;

class AutoInputComponent : public InputComponent
{
public:
  bool isHit(Command cmd) const override;
  bool isHeld(Command cmd) const override;
  bool isReleased(Command cmd) const override;

protected:
  void hit(Command cmd);
  void hold(Command cmd);
  void release(Command cmd);

  void updateDelegate(std::uint32_t, Game &) override;

private:
  std::vector<Command> mHitCommands;
  std::vector<Command> mHeldCommands;
  std::vector<Command> mReleasedCommands;
};

} // namespace game

#endif // AUTOINPUTCOMPONENT_H
