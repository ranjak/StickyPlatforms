#ifndef INPUTSENDERCOMPONENT_H
#define INPUTSENDERCOMPONENT_H

#include "component.h"
#include <array>

namespace game
{

class InputComponent;

class InputSenderComponent : public Component
{
public:
  InputSenderComponent(const InputComponent &input);

  void updateDelegate(std::uint32_t step, Game &game) override;

private:
  // Ignore keys that don't affect gameplay (pause, etc.)
  //enum GameKeys
  //{
  //  UP,
  //  DOWN,
  //  LEFT,
  //  RIGHT,
  //  JUMP,
  //  SWORD,
  //  NUM_KEYS
  //};
  //std::array<bool, NUM_KEYS> mPressedKeys;
  const InputComponent &mInput;
};

} // namespace game

#endif // INPUTSENDERCOMPONENT_H