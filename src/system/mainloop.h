#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "display.h"
#include "inputhandler.h"
#include "gamestate.h"
#include <cstdint>
#include <string>

namespace game {

class MainLoop
{
public:
  MainLoop(const std::string &initialLevel="assets/maps/test.tmx");

  MainLoop(const MainLoop &) = delete;
  MainLoop& operator=(const MainLoop &) = delete;

  void run();

private:
  void updateStats(std::uint32_t frameTime);
  void logStats();

private:
  Display mDisplay;
  InputHandler mInput;
  GameState mGame;

  std::uint32_t mMaxFrameTime;
  std::uint32_t mAccuFrameTimes;
  int mNumFrameTimes;
};

} // namespace game

#endif // MAINLOOP_H
