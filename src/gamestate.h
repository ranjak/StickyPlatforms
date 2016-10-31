#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gamecommands.h"
#include "camera.h"
#include "world/level.h"
#include "uipanel.h"
#include "loadlevelstate.h"
#include <cstdint>
#include <memory>
#include <string>

namespace game {

extern const std::uint32_t TIMESTEP;

class InputHandler;
class Display;

/**
 * @brief The GameState class describes the state of the game at a given moment.
 * It holds all the data that makes up said state.
 * It is responsible for updating the state.
 */
class GameState
{
public:
  enum class State {
    PLAYING,
    LOADING,
    VICTORY
  };

  /**
   * @brief current Get a reference to the current game.
   */
  static GameState &current();

  GameState(Display &display, InputHandler &input, int camW, int camH, const std::string &initialLevel);

  /**
   * @brief update Update this state by the given amount of time.
   * @param step Amount of virtual time in milliseconds by which to advance the state.
   */
  void update(std::uint32_t step);

  void draw(Display& target);

  void setPlayingState();
  /**
   * @brief setLoadingState Display an on-screen message and prepare to load the next level.
   * Called when the player clears a level (or dies).
   * @param victory Has the player cleared the level, or is he dead?
   * @param nextLevel Level to load. If left empty, reload the current one.
   */
  void setLoadingState(bool victory, const std::string &nextLevel="");

  /**
   * @brief changeLevel Set a level file to be loaded at next update, replacing the current level.
   * @param levelFile Path to the level file (relative to the game's executable directory)
   */
  void changeLevel(const std::string &levelFile);
  // Load the given level right away, and spawn the player.
  void loadLevel(const std::string &levelFile);

  GameCommands &getCommands();
  const GameCommands &getCommands() const;

  Level& getLevel();
  const Level& getLevel() const;

  Camera &getCamera();
  const Camera &getCamera() const;

  UIPanel &getUI();

  Display &getDisplay();

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
  // Level to be loaded at next update
  std::string mNextLevel;
  // Camera that tracks the player
  Camera mCamera;
  // Game UI
  UIPanel mUI;
  // Simulated game time, advances every time update() is called
  std::uint32_t mGameTime;
  // State the game is currently in
  State mState;
  LoadLevelState mLoadingState;

  Display &mDisplay;
};

} //namespace game

#endif // GAMESTATE_H
