#ifndef GAME_H
#define GAME_H

#include "gamecommands.h"
#include "camera.h"
#include "world/level.h"
#include "uipanel.h"
#include "gamestate.h"
#include <cstdint>
#include <memory>
#include <string>
#include <array>
#include <type_traits>

namespace game {

extern const std::uint32_t TIMESTEP;

class InputHandler;
class Display;

/**
 * @brief The Game class describes the state of the game at a given moment.
 * It holds all the data that makes up said state.
 * It is responsible for updating the state.
 */
class Game
{
public:
  enum State {
    PLAYING,
    LOADING,
    CLEARED,
    PAUSED,
    NB_STATES
  };

  /**
   * @brief current Get a reference to the current game.
   */
  static Game &current();

  Game(Display &display, InputHandler &input, int camW, int camH, const std::string &initialLevel);

  /**
   * @brief update Update this state by the given amount of time.
   * @param step Amount of virtual time in milliseconds by which to advance the state.
   */
  void update(std::uint32_t step);

  void draw(Display& target);

  template<typename state_t, typename... Args>
  void setState(Args... args)
  {
    static_assert(std::is_base_of<GameState, state_t>::value, "setState: state_t must be a subclass of GameState");

    for (std::unique_ptr<GameState> &state : mStates) {
      state_t *castState = dynamic_cast<state_t *>(state.get());

      if (castState) {

        if (mState)
          mState->exit();

        castState->enter(args...);
        mState = castState;

        return;
      }
    }
  }

  /**
   * @brief changeLevel Set a level file to be loaded at next update, replacing the current level.
   * @param levelFile Path to the level file (relative to the game's executable directory)
   */
  void changeLevel(const std::string &levelFile);
  // Load the given level right away, and spawn the player.
  void loadLevel(const std::string &levelFile);

  const std::string &getInitialLevel() const;

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
  static Game *currentGame;

  // Game commands bindings and status
  GameCommands mCommands;
  Display &mDisplay;

  // Current level
  std::unique_ptr<Level> mLevel;
  // Level to be loaded at next update
  std::string mNextLevel;
  std::string mInitialLevel;
  // Camera that tracks the player
  Camera mCamera;
  // Game UI
  UIPanel mUI;
  // Possible game states
  std::array<std::unique_ptr<GameState>, State::NB_STATES> mStates;
  // State the game is currently in
  GameState *mState;

};

} //namespace game

#endif // GAME_H
