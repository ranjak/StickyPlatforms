#include <iostream>
#include <thread>
#include "SDL.h"
#include "log.h"
#include "inputhandler.h"
#include "gamestate.h"
#include "display.h"


int main(int argc, char *argv[])
{
  std::cout << "Hello World!" << std::endl;

  game::Display display;
  game::InputHandler input;
  game::GameState game;

  display.init();
  glog(game::Log::Priority::DBG, "Initialization OK");

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
      game.update(game::TIMESTEP, input);
      gameTime += game::TIMESTEP;
    }

    display.render(game);

    std::this_thread::yield();
  }

  return 0;
}
