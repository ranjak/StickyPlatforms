#include "uipanel.h"
#include "display.h"
#include "log.h"

namespace game {

UIPanel::UIPanel(const Vector<float> &size) :
  mWidgets(),
  mSize(size)
{

}

void UIPanel::draw(Display &display)
{
  display.setLogicalSize(static_cast<int>(mSize.x), static_cast<int>(mSize.y));

  for (const std::unique_ptr<UIWidget> &widget : mWidgets) {
    if (!widget->isHidden())
      widget->draw(display);
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

} // namespace game
