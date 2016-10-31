#ifndef UTIL_SDL_H
#define UTIL_SDL_H

#include "SDL.h"
#include "rect.h"

struct SDL_Rect;
struct SDL_Color;

namespace game {

class Color;

template<typename T>
SDL_Rect getSdlRect(const Rect<T> &rect);

SDL_Rect getSdlRect(const Rect<float> &rect);

template<typename T>
Rect<T> getRect(const SDL_Rect &sdlRect)
{
  return Rect<T>(static_cast<T>(sdlRect.x), static_cast<T>(sdlRect.y), static_cast<T>(sdlRect.w), static_cast<T>(sdlRect.h));
}

SDL_Color getSdlColor(const Color &color);

}

#endif // UTIL_SDL_H
