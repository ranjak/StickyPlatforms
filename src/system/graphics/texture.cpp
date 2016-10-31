#include "texture.h"
#include "display.h"
#include "log.h"
#include "util_sdl.h"
#include "rect.h"
#include "SDL.h"

namespace game {


Texture::Texture(Display &renderer) :
  mTexture(nullptr, SDL_DestroyTexture),
  mSize(),
  mRenderer(renderer)
{
}

Texture::Texture(Display &renderer, SDL_Surface *surface) :
  Texture(renderer)
{
  reset(surface);
}

void Texture::draw(Display &target, int x, int y)
{
  // Draw the texture without resizing it
  draw(target, Rect<int>(x, y, mSize.x, mSize.y), Rect<int>(0, 0, mSize.x, mSize.y));
}

void Texture::draw(Display &target, const Rect<int> &dest)
{
  draw(target, dest, Rect<int>(0, 0, mSize.x, mSize.y));
}

void Texture::reset(SDL_Surface *surface)
{
  mTexture.reset(SDL_CreateTextureFromSurface(mRenderer.getRenderer(), surface));

  if (!mTexture) {
    Log::getGlobal().get(Log::WARNING) << "Texture: creation from SDL surface failed: "<<SDL_GetError()<<std::endl;
  }

  else if (SDL_QueryTexture(mTexture.get(), nullptr, nullptr, &mSize.x, &mSize.y) < 0) {
    Log::getGlobal().get(Log::WARNING) << "SDL Texture error: "<<SDL_GetError()<<std::endl;
  }
}

template<typename T>
void Texture::draw(Display &target, const Rect<T> &dest, const Rect<T> &source) const
{
  SDL_Rect destRect = getSdlRect(dest);
  SDL_Rect srcRect = getSdlRect(source);

  SDL_RenderCopy(mRenderer.getRenderer(), mTexture.get(), &srcRect, &destRect);
}

template void Texture::draw(Display &target, const Rect<int> &dest, const Rect<int> &source) const;
template void Texture::draw(Display &target, const Rect<float> &dest, const Rect<float> &source) const;


} // namespace game
