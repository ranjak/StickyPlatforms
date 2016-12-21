#ifndef TEXT_H
#define TEXT_H

#include "texture.h"
#include "color.h"
#include "SDL_ttf.h"
#include <string>
#include <memory>

namespace game {

class Display;
template<typename T> class Vector;

/**
 * @brief Text line rendered using SDL_ttf.
 * The text size automatically takes into account the renderer's scaling factor,
 * so that text is always rendered relative to the screen size,
 * instead of being stretched by the renderer.
 */
class Text : public Texture
{
public:
  static const std::string defaultFont;

  using FontUptr = std::unique_ptr<TTF_Font, void (*)(TTF_Font *)>;

  // TODO don't open the font if it's already used by another Text.
  Text(Display &renderer, const std::string &text, int fontSize, const std::string &fontFile=defaultFont, const Color &color=Color::WHITE);

  void setSize(int size);

  /**
   * @brief getSize Reports the constant, resolution-independent size of the text line.
   */
  Vector<int> getSize() const;

  void setText(const std::string &text, const Color &color=Color::WHITE);

  void draw(Display &target, int x, int y) override;
  void draw(Display &target, const Vector<float> &pos);

private:
  int mSize;
  float mRenderScale;
  FontUptr mFont;
  std::string mFontFile;
  std::string mText;
  Color mColor;
};

} // namespace game

#endif // TEXT_H
