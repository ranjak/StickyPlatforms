#ifndef UIWIDGET_H
#define UIWIDGET_H

#include "gamevector.h"
#include <string>

namespace game {

class Display;

class UIWidget
{
public:
  UIWidget(const Vector<float> &pos={}, const std::string &name="", bool hidden=false);

  virtual void draw(Display &display) = 0;

  void setPosition(float x, float y);
  virtual void setSize(float w, float h);

  virtual Vector<float> getSize() const = 0;
  const Vector<float> &getPosition() { return mPos; }

  void setHidden(bool hidden);
  bool isHidden() const { return mHidden; }

public:
  const std::string name;
protected:
  bool mHidden;
  Vector<float> mPos;
};

} // namespace game

#endif // UIWIDGET_H
