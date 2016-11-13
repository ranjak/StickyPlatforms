#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <cstdint>

namespace game {

class GameCommands;

/**
 * @brief A particular state the running game can be in.
 * For instance: playing, paused, loading...
 */
class GameState
{
public:

  virtual ~GameState() = 0;

  virtual void handleInput(GameCommands &commands);

  virtual void update(std::uint32_t step) {}

  void enter() {}

  virtual void exit() {}
};

inline GameState::~GameState() {}

} // namespace game

#endif // GAMESTATE_H
