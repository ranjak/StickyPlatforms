#include <iostream>
#include "SDL.h"
#include "log.h"
#include "inputhandler.h"
#include "renderer.h"


int main(int argc, char *argv[])
{
  std::cout << "Hello World!" << std::endl;

  game::Display display;
  game::InputHandler input;

  display.init();
  glog(game::Log::Priority::DBG, "Initialization OK");

  while (!input.quitRequested()) {
    input.handle();
    // Logic
    display.render();
    SDL_Delay(10);
  }

  return 0;
}
