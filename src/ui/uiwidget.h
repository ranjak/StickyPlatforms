#ifndef UIWIDGET_H
#define UIWIDGET_H

#include "rect.h"
#include <string>

namespace game {

class Display;

class UIWidget
{
public:
  UIWidget(const Rect<float> &bounds, const std::string &name="");

  virtual void draw(Display &display) const = 0;

  void setPosition(float x, float y);
  virtual void setSize(float w, float h);

  void setHidden(bool hidden);
  bool isHidden() const { return mHidden; }

public:
  const std::string name;
protected:
  Rect<float> mBounds;
  bool mHidden;
};

} // namespace game

#endif // UIWIDGET_H
