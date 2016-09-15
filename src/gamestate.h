#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <cstdint>
#include <memory>
#include "gamecommands.h"
#include "camera.h"
#include "world/level.h"

namespace game {

extern const std::uint32_t TIMESTEP;

class Level;
class InputHandler;

/**
 * @brief The GameState class describes the state of the game at a given moment.
 * It holds all the data that makes up said state.
 * It is responsible for updating the state.
 */
class GameState
{
public:
  GameState(Display &display, InputHandler &input);

  /**
   * @brief update Update this state by the given amount of time.
   * @param step Amount of virtual time in milliseconds by which to advance the state.
   */
  void update(std::uint32_t step);

  void draw(Display& target) const;

  GameCommands &getCommands();

  Level& getLevel();

  Camera &getCamera();
  const Camera &getCamera() const;

private:
  // Game commands bindings and status
  GameCommands mCommands;
  // Current level
  std::unique_ptr<Level> mLevel;
  Camera mCamera;
};

} //namespace game

#endif // GAMESTATE_H
