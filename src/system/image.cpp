#include "image.h"
#include "display.h"
#include "util_sdl.h"
#include "rect.h"
#include "SDL.h"
#include "SDL_image.h"
#include <stdexcept>

namespace game {


Image::Image(Display &renderer, const std::string &filename) :
  Graphics(),
  mTexture(nullptr, SDL_DestroyTexture),
  mRenderer(renderer)
{
  // TODO initialize SDL_image at program start
  SDL_Surface* img = IMG_Load(filename.c_str());

  if (img == nullptr)
    throw std::runtime_error(std::string("Couldn't load image ") + filename + ": " + IMG_GetError());

  mTexture.reset(SDL_CreateTextureFromSurface(mRenderer.getRenderer(), img));

  SDL_FreeSurface(img);
  if (mTexture.get() == nullptr)
    throw std::runtime_error(std::string("Couldn't create texture: ") + SDL_GetError());
}

void Image::draw(Display &target, int x, int y) const
{
  // Draw the texture without resizing it
  Rect<int> destRect(x, y, 0, 0);

  if (SDL_QueryTexture(mTexture.get(), nullptr, nullptr, &destRect.w, &destRect.h) < 0)
    throw std::runtime_error(std::string("SDL Texture error: ") + SDL_GetError());

  draw(target, destRect);
}

void Image::draw(Display &target, const Rect<int> &dest) const
{
  SDL_Rect drawRect = getSdlRect(dest);

  SDL_RenderCopy(mRenderer.getRenderer(), mTexture.get(), nullptr, &drawRect);
}

}
