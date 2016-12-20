#include "textwidget.h"
#include "rect.h"

namespace game {

TextWidget::TextWidget(Display &display, const std::string &name, const std::string &text, int size, const Vector<float> &pos, const Color &color, const std::string &font) :
  UIWidget(pos, name),
  mText(display, text, size, font, color)
{

}

void TextWidget::draw(Display &display, const Vector<float> &offset)
{
  mText.draw(display, mPos + offset);
}

Vector<float> TextWidget::getSize() const
{
  return Vector<float>(mText.getSize());
}

void TextWidget::setText(const std::string &text)
{
  mText.setText(text);
}


} // namespace game
