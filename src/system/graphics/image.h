#ifndef IMAGE_H
#define IMAGE_H

#include "gamevector.h"
#include "texture.h"
#include <string>
#include <memory>

struct SDL_Surface;

namespace game {


class Image : public Texture
{
public:
  Image(Display& renderer, const std::string& filename);

private:
  std::unique_ptr<struct SDL_Surface, void(*)(struct SDL_Surface*)> loadImage(const std::string& filename);
};

}

#endif // IMAGE_H
