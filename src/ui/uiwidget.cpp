#include "uiwidget.h"
#include "graphics.h"

namespace game {

UIWidget::UIWidget(const Rect<float> &bounds, const std::string &name) :
  name(name),
  mBounds(bounds),
  mHidden(false)
{

}

void UIWidget::setPosition(float x, float y)
{
  mBounds.x = x;
  mBounds.y = y;
}

void UIWidget::setSize(float w, float h)
{
  mBounds.w = w;
  mBounds.h = h;
}

void UIWidget::setHidden(bool hidden)
{
  mHidden = hidden;
}

} // namespace game
