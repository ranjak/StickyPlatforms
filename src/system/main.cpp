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

  display.init();
  glog(game::Log::Priority::DBG, "Initialization OK");

  display.setCameraSize(320, 240);
  game::GameState game(display);

  // Simulated game time. Increases by a fixed amount at every game update.
  Uint32 gameTime = 0;
  // Time at which the main loop was started.
  Uint32 startTime = SDL_GetTicks();

  while (!input.quitRequested()) {

    input.handle();
    if (input.quitRequested())
      break;

    Uint32 realTimeElasped = SDL_GetTicks() - startTime;
    while (realTimeElasped > gameTime) {
      game.update(game::TIMESTEP, &input);
      gameTime += game::TIMESTEP;
    }

    display.render(game);

    // Sleep until it's time to update the game again
    std::int32_t idleTime = gameTime - (SDL_GetTicks() - startTime);
    if (idleTime > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(idleTime));
  }

  return 0;
}
