#include "uiwidget.h"
#include "graphics.h"

namespace game {

UIWidget::UIWidget(const Vector<float> &pos, const std::string &name, bool hidden) :
  name(name),
  mHidden(hidden),
  mPos(pos)
{

}

void UIWidget::setPosition(float x, float y)
{
  mPos.x = x;
  mPos.y = y;
}

void UIWidget::setSize(float w, float h)
{
}

void UIWidget::setHidden(bool hidden)
{
  mHidden = hidden;
}

} // namespace game
