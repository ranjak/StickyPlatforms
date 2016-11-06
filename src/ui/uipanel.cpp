#include "uipanel.h"
#include "display.h"
#include "log.h"

namespace game {

UIPanel::UIPanel(const Vector<float> &size) :
  mWidgets(),
  mSize(size)
{

}

void UIPanel::draw(Display &display, const Vector<float> &offset)
{
  display.setLogicalSize(static_cast<int>(mSize.x), static_cast<int>(mSize.y));

  for (const std::unique_ptr<UIWidget> &widget : mWidgets) {
    if (!widget->isHidden())
      widget->draw(display, offset);
  }
}

UIWidget *UIPanel::getByName(const std::string &widgetName)
{
  for (const std::unique_ptr<UIWidget> &widget : mWidgets) {
    if (widget->name == widgetName)
      return widget.get();
  }

  return nullptr;
}

void UIPanel::addWidget(std::unique_ptr<UIWidget> widget)
{
  mWidgets.push_back(std::move(widget));
}

void UIPanel::setCentered(const std::string &widgetName)
{
  UIWidget *widget = getByName(widgetName);

  if (widget) {
    Vector<float> wSize = widget->getSize();
    widget->setPosition(mSize.x/2.f - wSize.x/2.f, mSize.y/2.f - wSize.y/2.f);
  }
  else
    Log::getGlobal().get(Log::WARNING) << "UIPanel: widget \"" << widgetName << "\" not found" << std::endl;
}

const Vector<float> &UIPanel::getSize() const
{
  return mSize;
}

} // namespace game
