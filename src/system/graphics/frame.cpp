#include "frame.h"
#include "util_sdl.h"
#include "SDL.h"
#include "rect.h"
#include "display.h"

namespace game {

Frame::Frame(int w, int h, const Color &color) :
  mColor(color),
  w(w), h(h)
{

}

void Frame::draw(Display &target, int x, int y) const
{
  draw(target, Rect<int>(x, y, w, h));
}

void Frame::draw(Display &target, const Rect<int> &dest) const
{
  SDL_Rect renderRect = getSdlRect(dest);

  // Don't try to render if size is invalid
  if (renderRect.w <= 0 || renderRect.h <= 0)
    return;

  SDL_Renderer* renderer = target.getRenderer();

  SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
  SDL_RenderDrawRect(renderer, &renderRect);
}

} // namespace game
