#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <cstdint>
#include <memory>
#include "gamecommands.h"
#include "inputhandler.h"
#include "display.h"
#include "world/level.h"

namespace game {

extern const std::uint32_t TIMESTEP;

class Level;

/**
 * @brief The GameState class describes the state of the game at a given moment.
 * It holds all the data that makes up said state.
 * It is responsible for updating the state.
 */
class GameState
{
public:
  GameState();

  /**
   * @brief update Update this state by the given amount of time.
   * @param step Amount of virtual time in milliseconds by which to advance the state.
   * @param input Object containing the player's input status.
   */
  void update(std::uint32_t step, const InputHandler* input);

  void draw(Display& target) const;

  bool isCommandPressed(Command cmd);

  Level& getLevel();

private:
  // Key bindings to game commands
  GameCommands mBindings;
  // Current snapshot of user input
  const InputHandler* mInputSnapshot;
  // Current level
  std::unique_ptr<Level> mLevel;
};

} //namespace game

#endif // GAMESTATE_H
