#include "display.h"
#include "log.h"
#include "SDL.h"

namespace game {


Display::Display() :
  window(nullptr),
  renderer(nullptr)
{

}

Display::~Display()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Display::init()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      game::error(std::string("Failed to initialize SDL: ")+SDL_GetError());
      throw std::exception();
  }

  if (SDL_CreateWindowAndRenderer(320, 240, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
      game::error(std::string("Couldn't create window and renderer: ") + SDL_GetError());
      throw std::exception();
  }
}

void Display::render(const GameState &game)
{
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

}
