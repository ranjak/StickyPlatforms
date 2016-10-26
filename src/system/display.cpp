#include "display.h"
#include "log.h"
#include "gamestate.h"
#include "SDL.h"
#include <stdexcept>

namespace game {


Display::Display(int winW, int winH) :
  mWindow(nullptr),
  mRenderer(nullptr),
  mCameraSize(winW, winH)
{
  init(winW, winH);
}

Display::~Display()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Display::init(int winW, int winH)
{
  // In debug mode, the game will stop responding on breakpoints, avoid annoying WM messages
#ifndef NDEBUG
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_PING, "0");
#endif

  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
      game::error(std::string("Failed to initialize SDL: ")+SDL_GetError());
      throw std::exception();
  }

  if (SDL_CreateWindowAndRenderer(winW, winH, SDL_WINDOW_RESIZABLE, &mWindow, &mRenderer)) {
      game::error(std::string("Couldn't create window and renderer: ") + SDL_GetError());
      throw std::exception();
  }
}

void Display::setCameraSize(int w, int h)
{
  mCameraSize.x = w;
  mCameraSize.y = h;
}

void Display::useWindowCoordinates()
{
  if (SDL_RenderSetScale(mRenderer, 1.f, 1.f) < 0)
    throw std::runtime_error(SDL_GetError());
}

void Display::render(const GameState &game)
{
  if (SDL_RenderSetLogicalSize(mRenderer, mCameraSize.x, mCameraSize.y) < 0)
    throw std::runtime_error(SDL_GetError());

  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
  SDL_RenderClear(mRenderer);

  game.draw(*this);

  SDL_RenderPresent(mRenderer);
}

SDL_Renderer *Display::getRenderer()
{
  return mRenderer;
}

Vector<int> Display::getWindowSize()
{
  Vector<int> size;
  SDL_GetWindowSize(mWindow, &size.x, &size.y);
  return size;
}

}
