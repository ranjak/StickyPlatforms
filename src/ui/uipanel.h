#ifndef UIPANEL_H
#define UIPANEL_H

#include "uiwidget.h"
#include "make_unique.h"
#include <vector>
#include <memory>
#include <string>

namespace game {

class Display;

class UIPanel
{
public:
  UIPanel();

  void draw(Display &display) const;

  UIWidget *getByName(const std::string &widgetName);

  template<typename T, typename... Args>
  void addWidget(Args&&... args)
  {
    mWidgets.push_back(std::make_unique<T>(std::forward<Args>(args)...));
  }

private:
  std::vector<std::unique_ptr<UIWidget>> mWidgets;
};

} // namespace game

#endif // UIPANEL_H
