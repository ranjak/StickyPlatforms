#include "util_sdl.h"
#include "rect.h"
#include "SDL.h"

namespace game {


template<typename T>
SDL_Rect getSdlRect(const Rect<T> &rect)
{
  return SDL_Rect { rect.x, rect.y, rect.w, rect.h };
}


template SDL_Rect getSdlRect(const Rect<int> &rect);
//template SDL_Rect getSdlRect(const Rect<float> &rect);

} // namespace game
