#ifndef GAMECOMMANDS_H
#define GAMECOMMANDS_H

#include "inputhandler.h"
#include <cstdint>

namespace game {

struct KeyBinding
{
  KeyBinding(std::uint32_t scancode, const ModifierKey &modifier = ModifierKey::NONE) : keyScancode(scancode), modifier(modifier) {}

  std::uint32_t keyScancode;
  ModifierKey modifier;
};

enum Command {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  JUMP,
  SWORD,
  RESET,
  PAUSE,
  TOGGLE_FULLSCREEN,
  NB_CMD
};

class GameCommands
{
public:
  GameCommands(InputHandler &input);

  void setBinding(Command cmdToBind, const KeyBinding &binding);

  const KeyBinding &getBinding(Command cmd) const;

  bool isHit(Command cmd) const;
  bool isHeld(Command cmd) const;
  bool isReleased(Command cmd) const;

  const InputHandler &getInput() const { return mInput; }

private:
  KeyBinding mBindings[NB_CMD];
  InputHandler &mInput;
};

}

#endif // GAMECOMMANDS_H
