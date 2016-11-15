#include "image.h"
#include "log.h"
#include "resource.h"
#include "SDL.h"
#include "SDL_image.h"

namespace game {


Image::Image(Display &renderer, const std::string &filename) :
  Texture(renderer, loadImage(filename).get())
{
}


std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)> Image::loadImage(const std::string &filename)
{
  // TODO initialize SDL_image at program start
  std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)> img(IMG_Load(Resource::getInstance().assetPath(filename).c_str()), SDL_FreeSurface);

  if (!img)
    game::error(std::string("Couldn't load image ") + filename + ": " + IMG_GetError());

  return img;
}

}
