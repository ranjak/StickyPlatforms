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
  Text(Display &renderer, const std::string &fontFile, int fontSize, const std::string &text, const Color &color=Color::WHITE);
};

} // namespace game

#endif // TEXT_H
