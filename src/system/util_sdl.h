#ifndef UTIL_SDL_H
#define UTIL_SDL_H

struct SDL_Rect;
struct SDL_Color;

namespace game {

template<typename T>
class Rect;
class Color;

template<typename T>
SDL_Rect getSdlRect(const Rect<T> &rect);

SDL_Rect getSdlRect(const Rect<float> &rect);

SDL_Color getSdlColor(const Color &color);

}

#endif // UTIL_SDL_H
