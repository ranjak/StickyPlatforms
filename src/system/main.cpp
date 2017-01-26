#include "mainloop.h"
#include "SDL.h"
#include <string>

namespace
{
const std::string defaultScene = "publicGame";
}

int main(int argc, char *argv[])
{
  // Get a username from command line
  if (argc < 2) {
    throw std::exception("Please provide a username as program argument.");
  }

  std::string multiplayerScene = (argc == 3) ? std::string(argv[2]) : defaultScene;

  game::MainLoop loop(std::string(argv[1]), multiplayerScene);

  loop.run();

  return 0;
}
