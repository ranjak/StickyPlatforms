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
  /**
   * @brief current Get a read-only reference to the current game.
   */
  static const GameState &current();

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

  /**
   * @brief now Get the current simulated game time since the game started.
   * @return The game time in milliseconds.
   */
  std::uint32_t now() const;

private:
  static GameState *currentGame;
  // Game commands bindings and status
  GameCommands mCommands;
  // Current level
  std::unique_ptr<Level> mLevel;
  // Camera that tracks the player
  Camera mCamera;
  // Simulated game time, advances every time update() is called
  std::uint32_t mGameTime;
};

} //namespace game

#endif // GAMESTATE_H
