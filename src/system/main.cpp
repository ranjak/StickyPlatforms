#include <iostream>
#include <thread>
#include <string>
#include "SDL.h"
#include "log.h"
#include "inputhandler.h"
#include "gamestate.h"
#include "display.h"


int main(int argc, char *argv[])
{
  game::Display display;
  game::InputHandler input;

  display.init(640, 480);
  glog(game::Log::Priority::DBG, "Initialization OK");

  game::GameState game(display, input, 320, 240);

  // Simulated game time. Increases by a fixed amount at every game update.
  Uint32 gameTime = 0;
  // Time at which the main loop was started.
  Uint32 startTime = SDL_GetTicks();

  while (!input.quitRequested()) {

    Uint32 realTimeElasped = SDL_GetTicks() - startTime;
    while (realTimeElasped > gameTime) {

      input.handle();
      if (input.quitRequested())
        break;

      game.update(game::TIMESTEP);
      gameTime += game::TIMESTEP;
    }

    if (input.quitRequested())
        break;

    display.render(game);

    // Sleep until it's time to update the game again
    std::int32_t idleTime = gameTime - (SDL_GetTicks() - startTime);
    if (idleTime > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(idleTime));
  }

  return 0;
}
