#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include "uiwidget.h"
#include "text.h"
#include "color.h"
#include "gamevector.h"
#include <string>

namespace game {

class Display;

class TextWidget : public UIWidget
{
public:
  TextWidget(Display &display, const std::string &name, const std::string &text, int size, const Vector<float> &pos={}, const Color &color=Color::WHITE, const std::string &font=Text::defaultFont);

  void draw(Display &display, const Vector<float> &offset={}) override;

  Vector<float> getSize() const override;

private:
  Text mText;
};

} // namespace game

#endif // TEXTWIDGET_H
