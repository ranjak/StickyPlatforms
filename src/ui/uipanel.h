#ifndef UIPANEL_H
#define UIPANEL_H

#include "uiwidget.h"
#include "make_unique.h"
#include "gamevector.h"
#include <vector>
#include <memory>
#include <string>

namespace game {

class Display;

class UIPanel
{
public:
  UIPanel(const Vector<float> &size);

  void draw(Display &display, const Vector<float> &offset={});

  UIWidget *getByName(const std::string &widgetName);

  template<typename T, typename... Args>
  void addWidget(Args&&... args)
  {
    mWidgets.push_back(std::make_unique<T>(std::forward<Args>(args)...));
  }

  void addWidget(std::unique_ptr<UIWidget> widget);

  void setCentered(const std::string &widgetName);

  const Vector<float> &getSize() const;

private:
  std::vector<std::unique_ptr<UIWidget>> mWidgets;
  Vector<float> mSize;
};

} // namespace game

#endif // UIPANEL_H
