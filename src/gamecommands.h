#ifndef GAMECOMMANDS_H
#define GAMECOMMANDS_H

#include <cstdint>

namespace game {

class InputHandler;

enum Command {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  JUMP,
  SWORD,
  NB_CMD
};

class GameCommands
{
public:
  GameCommands(InputHandler &input);

  void setBinding(Command cmdToBind, std::uint32_t scancode);

  std::uint32_t getBinding(Command cmd) const;

  bool isHit(Command cmd) const;
  bool isHeld(Command cmd) const;
  bool isReleased(Command cmd) const;

private:
  std::uint32_t mBindings[NB_CMD];
  InputHandler &mInput;
};

}

#endif // GAMECOMMANDS_H
