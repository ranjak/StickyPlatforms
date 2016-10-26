#include "texture.h"
#include "display.h"
#include "log.h"
#include "util_sdl.h"
#include "rect.h"
#include "SDL.h"

namespace game {


Texture::Texture(Display &renderer, SDL_Surface *surface) :
  mTexture(nullptr, SDL_DestroyTexture),
  mSize(),
  mRenderer(renderer)
{
  mTexture.reset(SDL_CreateTextureFromSurface(renderer.getRenderer(), surface));

  if (!mTexture) {
    game::error(std::string("Couldn't create texture: ") + SDL_GetError());
  }

  if (SDL_QueryTexture(mTexture.get(), nullptr, nullptr, &mSize.x, &mSize.y) < 0) {
    game::error(std::string("SDL Texture error: ") + SDL_GetError());
  }
}

void Texture::draw(Display &target, int x, int y) const
{
  // Draw the texture without resizing it
  draw(target, Rect<int>(x, y, mSize.x, mSize.y));
}

void Texture::draw(Display &target, const Rect<int> &dest) const
{
  draw(target, dest, Rect<int>(0, 0, mSize.x, mSize.y));
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
