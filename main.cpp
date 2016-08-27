#include <iostream>
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

  while (!input.quitRequested()) {
    input.handle();
    if (input.quitRequested())
      break;

    game.update(game::TIMESTEP, input);
    display.render(game);
    SDL_Delay(10);
  }

  return 0;
}
