#include <iostream>
#include <thread>
#include <string>
#include "SDL.h"
#include "log.h"
#include "inputhandler.h"
#include "gamestate.h"
#include "display.h"
#include "mainloop.h"


int main(int argc, char *argv[])
{
  game::MainLoop loop;

  loop.run();

  return 0;
}
