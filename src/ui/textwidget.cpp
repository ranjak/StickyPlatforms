#include "textwidget.h"
#include "rect.h"

namespace game {

TextWidget::TextWidget(Display &display, const std::string &name, const std::string &text, int size, const Vector<float> &pos, const Color &color, const std::string &font) :
  UIWidget(pos, name),
  mText(display, text, size, font, color)
{

}

void TextWidget::draw(Display &display)
{
  mText.draw(display, mPos);
}

Vector<float> TextWidget::getSize() const
{
  return Vector<float>(mText.getSize());
}


} // namespace game
