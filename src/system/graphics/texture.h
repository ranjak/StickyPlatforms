#ifndef TEXTURE_H
#define TEXTURE_H

#include "graphics.h"
#include "gamevector.h"
#include <memory>

struct SDL_Texture;
struct SDL_Surface;

namespace game {

class Display;

class Texture : public Graphics
{
public:
  using Graphics::draw;

  Texture(Display &renderer, SDL_Surface *surface);

  void draw(Display &target, int x, int y) const override;

  void draw(Display &target, const Rect<int> &dest) const override;

  template<typename T>
  void draw(Display &target, const Rect<T> &dest, const Rect<T> &source) const;

  const Vector<int> &getSize() const { return mSize; }

private:
  std::unique_ptr<struct SDL_Texture, void(*)(struct SDL_Texture*)> mTexture;
  Vector<int> mSize;
  Display &mRenderer;
};

} // namespace game

#endif // TEXTURE_H
