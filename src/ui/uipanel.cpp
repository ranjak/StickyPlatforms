#include "uipanel.h"

namespace game {

UIPanel::UIPanel()
{

}

void UIPanel::draw(Display &display) const
{
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

} // namespace game
