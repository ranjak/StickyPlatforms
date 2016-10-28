#ifndef TEXT_H
#define TEXT_H

#include "texture.h"
#include "color.h"
#include <string>

namespace game {

class Display;

class Text : public Texture
{
public:
  static const std::string defaultFont;

  // TODO don't open the font if it's already used by another Text.
  Text(Display &renderer, const std::string &text, int fontSize, const std::string &fontFile=defaultFont, const Color &color=Color::WHITE);
};

} // namespace game

#endif // TEXT_H
