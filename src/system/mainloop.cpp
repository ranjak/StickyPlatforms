#include "mainloop.h"
#include "log.h"
#include "sdlinputhandler.h"
#include "game.h"
#include "display.h"
#include "make_unique.h"
#include "SDL.h"
#include <thread>
#include <algorithm>


namespace game {


MainLoop *MainLoop::instance = nullptr;

void MainLoop::requestExit()
{
  instance->setExit();
}

void MainLoop::setBlocked(bool blocked)
{
  if (blocked)
    instance->block();
  else
    instance->unblock();
}

MainLoop::MainLoop(const std::string &username, const std::string &scene) :
  mDisplay(640, 360),
  mInput(std::make_unique<SDLInputHandler>()),
  mGame(mDisplay, *mInput, 640, 360, username, scene),
  mExitRequested(false),
  mStartTime(0),
  mBlockTimestamp(0),
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
  mStartTime = SDL_GetTicks();

  while (!mExitRequested) {

    Uint32 realTimeElasped = SDL_GetTicks() - mStartTime;

    while (realTimeElasped > gameTime && !mExitRequested) {

      mInput->handle();

      if (mExitRequested)
        break;

      mGame.update(TIMESTEP);
      gameTime += TIMESTEP;
    }

    if (mExitRequested)
        break;

    mDisplay.render(mGame);

    // Framerate statistics
    Uint32 frameTime = SDL_GetTicks() - realTimeElasped - mStartTime;
    updateStats(frameTime);

    if (mAccuFrameTimes >= 500)
      logStats();
  }
}

void MainLoop::setExit()
{
  mExitRequested = true;
}

void MainLoop::block()
{
  mBlockTimestamp = SDL_GetTicks();
}

void MainLoop::unblock()
{
  mStartTime += (SDL_GetTicks() - mBlockTimestamp);
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
