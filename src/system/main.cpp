#include "mainloop.h"
#include <string>


int main(int argc, char *argv[])
{
  game::MainLoop loop;

  // Get a map file from command line
  if (argc == 2) {
    loop.setInitialLevel(std::string(argv[1]));
  }

  loop.run();

  return 0;
}
