#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "display.h"
#include "inputhandler.h"
#include "game.h"
#include "stormancerconnection.h"
#include <cstdint>
#include <string>
#include <memory>

namespace game {

class MainLoop
{
public:
  static void requestExit();
  static void setBlocked(bool blocked);

  MainLoop(const std::string & username, const std::string & scene);
  MainLoop(const MainLoop &) = delete;
  MainLoop& operator=(const MainLoop &) = delete;

  void setInitialLevel(const std::string &level);

  void run();

  void setExit();

  /**
   * @brief block Call this function when the main loop is about to be blocked.
   * Call \c unblock() when it resumes.
   * This allows the loop to adjust its internal timer to account for the time when it was blocked.
   */
  void block();
  void unblock();

private:
  void updateStats(std::uint32_t frameTime);
  void logStats();

private:
  static MainLoop *instance;

  Display mDisplay;
  std::unique_ptr<InputHandler> mInput;
  Game mGame;

  bool mExitRequested;
  std::uint32_t mStartTime;
  std::uint32_t mBlockTimestamp;

  std::uint32_t mMaxFrameTime;
  std::uint32_t mAccuFrameTimes;
  int mNumFrameTimes;
};

} // namespace game

#endif // MAINLOOP_H
