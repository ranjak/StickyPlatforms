#ifndef IMAGE_H
#define IMAGE_H

#include "graphics.h"
#include <iostream>
#include <memory>

struct SDL_Texture;

namespace game {


class Image : public Graphics
{
public:
  Image(Display& renderer, const std::string& filename);

  void draw(Display &target, int x, int y) const override;

  void draw(Display &target, const Rect<int> &dest) const override;

private:
  std::unique_ptr<struct SDL_Texture, void(*)(struct SDL_Texture*)> mTexture;
  Display& mRenderer;
};

}

#endif // IMAGE_H
