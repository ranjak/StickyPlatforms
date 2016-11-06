#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "display.h"
#include "inputhandler.h"
#include "game.h"
#include <cstdint>
#include <string>

namespace game {

class MainLoop
{
public:
  static void requestExit();

  MainLoop(const std::string &initialLevel="assets/maps/level1.tmx");

  MainLoop(const MainLoop &) = delete;
  MainLoop& operator=(const MainLoop &) = delete;

  void setInitialLevel(const std::string &level);

  void run();

  void setExit();

private:
  void updateStats(std::uint32_t frameTime);
  void logStats();

private:
  static MainLoop *instance;

  Display mDisplay;
  InputHandler mInput;
  Game mGame;

  bool mExitRequested;

  std::uint32_t mMaxFrameTime;
  std::uint32_t mAccuFrameTimes;
  int mNumFrameTimes;
};

} // namespace game

#endif // MAINLOOP_H
