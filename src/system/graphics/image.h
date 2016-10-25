#ifndef IMAGE_H
#define IMAGE_H

#include "graphics.h"
#include "gamevector.h"
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

  void draw(Display &target, const Rect<int> &dest, const Rect<int> &source) const;

  const Vector<int> &size() const { return mSize; }

private:
  std::unique_ptr<struct SDL_Texture, void(*)(struct SDL_Texture*)> mTexture;
  Display& mRenderer;
  Vector<int> mSize;
};

}

#endif // IMAGE_H
