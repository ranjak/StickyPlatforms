#ifndef GAMECOMMANDS_H
#define GAMECOMMANDS_H

#include <cstdint>

namespace game {

enum Command {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  NB_CMD
};

class GameCommands
{
public:
  GameCommands();

  void setBinding(Command cmdToBind, std::uint32_t scancode);

  std::uint32_t getBinding(Command cmd);

private:
  std::uint32_t bindings[NB_CMD];
};

}

#endif // GAMECOMMANDS_H
