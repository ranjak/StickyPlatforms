#ifndef REMOTEINPUTCOMPONENT_H
#define REMOTEINPUTCOMPONENT_H

#include "autoinputcomponent.h"

namespace game
{

class RemoteInputComponent : public AutoInputComponent
{
public:
  void hitCommand(int key);
  void releaseCommand(int key);

protected:
  void updateDelegate(std::uint32_t, Game &) override;
};

} // namespace game

#endif // REMOTEINPUTCOMPONENT_H