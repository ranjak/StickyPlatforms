#include "util_sdl.h"
#include "rect.h"
#include "color.h"
#include "SDL.h"

namespace game {


template<typename T>
SDL_Rect getSdlRect(const Rect<T> &rect)
{
  return SDL_Rect { rect.x, rect.y, rect.w, rect.h };
}

template SDL_Rect getSdlRect(const Rect<int> &rect);

SDL_Rect getSdlRect(const Rect<float> &rect)
{
  return SDL_Rect { static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.w), static_cast<int>(rect.h) };
}


SDL_Color getSdlColor(const Color &color)
{
  return SDL_Color { color.r, color.g, color.b, color.a };
}

} // namespace game
