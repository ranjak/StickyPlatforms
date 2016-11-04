#include "mainloop.h"
#include "log.h"
#include "inputhandler.h"
#include "game.h"
#include "display.h"
#include "SDL.h"
#include <thread>
#include <algorithm>

namespace game {


MainLoop::MainLoop(const std::string &initialLevel) :
  mDisplay(1280, 720),
  mInput(),
  mGame(mDisplay, mInput, 640, 360, initialLevel),
  mMaxFrameTime(0),
  mAccuFrameTimes(0),
  mNumFrameTimes(0)
{
}

void MainLoop::setInitialLevel(const std::string &level)
{
  mGame.changeLevel(level);
}

void MainLoop::run()
{
  // Simulated game time. Increases by a fixed amount at every game update.
  Uint32 gameTime = 0;
  // Time at which the main loop was started.
  Uint32 startTime = SDL_GetTicks();

  while (!mInput.quitRequested()) {

    Uint32 realTimeElasped = SDL_GetTicks() - startTime;
    while (realTimeElasped > gameTime) {

      mInput.handle();
      if (mInput.quitRequested())
        break;

      mGame.update(TIMESTEP);
      gameTime += TIMESTEP;
    }

    if (mInput.quitRequested())
        break;

    mDisplay.render(mGame);

    Uint32 frameTime = SDL_GetTicks() - realTimeElasped - startTime;
    updateStats(frameTime);

    if (mAccuFrameTimes >= 500)
      logStats();

    // Sleep until it's time to update the game again
//    std::int32_t idleTime = gameTime - (SDL_GetTicks() - startTime);
//    if (idleTime > 0)
//      std::this_thread::sleep_for(std::chrono::milliseconds(idleTime));
  }
}

void MainLoop::updateStats(uint32_t frameTime)
{
  mMaxFrameTime = std::max(mMaxFrameTime, frameTime);
  mAccuFrameTimes += frameTime;
  mNumFrameTimes++;
}

void MainLoop::logStats()
{
  float avgFrame = static_cast<float>(mAccuFrameTimes) / static_cast<float>(mNumFrameTimes);
  float fps = 1000.f / avgFrame;

  glog(Log::INFO, "FPS: " << fps << ", Avg frame: " << avgFrame << " ms, Max frame: " << mMaxFrameTime << " ms");

  mMaxFrameTime = 0;
  mAccuFrameTimes = 0;
  mNumFrameTimes = 0;
}

} // namespace game
