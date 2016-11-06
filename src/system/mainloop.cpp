#include "mainloop.h"
#include "log.h"
#include "inputhandler.h"
#include "game.h"
#include "display.h"
#include "SDL.h"
#include <thread>
#include <algorithm>

namespace game {


MainLoop *MainLoop::instance = nullptr;

MainLoop &MainLoop::get()
{
  return *instance;
}

MainLoop::MainLoop(const std::string &initialLevel) :
  mDisplay(1280, 720),
  mInput(),
  mGame(mDisplay, mInput, 640, 360, initialLevel),
  mExitRequested(false),
  mMaxFrameTime(0),
  mAccuFrameTimes(0),
  mNumFrameTimes(0)
{
  if (instance)
    game::error("MainLoop: Cannot create a new instance (there can be only one).");
  else
    instance = this;
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

  while (!mExitRequested) {

    Uint32 realTimeElasped = SDL_GetTicks() - startTime;

    while (realTimeElasped > gameTime && !mExitRequested) {

      mInput.handle();

      if (mExitRequested)
        break;

      mGame.update(TIMESTEP);
      gameTime += TIMESTEP;
    }

    if (mExitRequested)
        break;

    mDisplay.render(mGame);

    // Framerate statistics
    Uint32 frameTime = SDL_GetTicks() - realTimeElasped - startTime;
    updateStats(frameTime);

    if (mAccuFrameTimes >= 500)
      logStats();
  }
}

void MainLoop::requestExit()
{
  mExitRequested = true;
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
