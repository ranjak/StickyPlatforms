#include "display.h"
#include "log.h"
#include "gamestate.h"
#include "SDL.h"
#include <stdexcept>

namespace game {


Display::Display() :
  mWindow(nullptr),
  mRenderer(nullptr)
{

}

Display::~Display()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Display::init()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      game::error(std::string("Failed to initialize SDL: ")+SDL_GetError());
      throw std::exception();
  }

  if (SDL_CreateWindowAndRenderer(320, 240, SDL_WINDOW_RESIZABLE, &mWindow, &mRenderer)) {
      game::error(std::string("Couldn't create window and renderer: ") + SDL_GetError());
      throw std::exception();
  }
}

void Display::setCameraSize(int w, int h)
{
  if (SDL_RenderSetLogicalSize(mRenderer, w, h) < 0)
    throw std::runtime_error(SDL_GetError());
}

void Display::render(const GameState &game)
{
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
  SDL_RenderClear(mRenderer);

  game.draw(*this);

  SDL_RenderPresent(mRenderer);
}

SDL_Renderer *Display::getRenderer()
{
  return mRenderer;
}

}
