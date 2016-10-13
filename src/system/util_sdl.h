#ifndef UTIL_SDL_H
#define UTIL_SDL_H

struct SDL_Rect;

namespace game {

template<typename T>
class Rect;

template<typename T>
SDL_Rect getSdlRect(const Rect<T> &rect);

}

#endif // UTIL_SDL_H
