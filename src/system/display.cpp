#include "display.h"
#include "log.h"
#include "game.h"
#include "util_sdl.h"
#include "SDL.h"
#include <stdexcept>

namespace game {


Display::Display(int winW, int winH, const std::string &windowTitle) :
  mWindow(nullptr),
  mRenderer(nullptr),
  mCameraSize(winW, winH),
  mWindowTitle(windowTitle),
  mFullscreen(false)
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

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      game::error(std::string("Failed to initialize SDL: ")+SDL_GetError());
      throw std::exception();
  }

  if (SDL_CreateWindowAndRenderer(winW, winH, SDL_WINDOW_RESIZABLE|SDL_WINDOW_FULLSCREEN_DESKTOP, &mWindow, &mRenderer)) {
      game::error(std::string("Couldn't create window and renderer: ") + SDL_GetError());
      throw std::exception();
  }
  mFullscreen = true;

  SDL_SetWindowTitle(mWindow, mWindowTitle.c_str());
}

void Display::setLogicalSize(int w, int h)
{
  if (SDL_RenderSetLogicalSize(mRenderer, w, h) < 0)
    throw std::runtime_error(SDL_GetError());
}

void Display::setScale(float scaleFactor)
{
  if (SDL_RenderSetScale(mRenderer, scaleFactor, scaleFactor) < 0)
    throw std::runtime_error(SDL_GetError());
}

void Display::toggleFullscreen()
{
  int result = SDL_SetWindowFullscreen(mWindow, mFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);

  if (result < 0)
    game::error(std::string("Couldn't set fullscreen mode: ") + SDL_GetError());
  else
    mFullscreen = !mFullscreen;
}

Rect<int> Display::getViewport()
{
  SDL_Rect viewport;
  SDL_RenderGetViewport(mRenderer, &viewport);
  return getRect<int>(viewport);
}

Vector<int> Display::getOutputSize()
{
  Vector<int> size;
  SDL_GetRendererOutputSize(mRenderer, &size.x, &size.y);

  return size;
}

Vector<float> Display::getScale()
{
  Vector<float> scale;
  SDL_RenderGetScale(mRenderer, &scale.x, &scale.y);

  return scale;
}

void Display::render(Game &game)
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

Vector<int> Display::getWindowSize()
{
  Vector<int> size;
  SDL_GetWindowSize(mWindow, &size.x, &size.y);
  return size;
}

}
