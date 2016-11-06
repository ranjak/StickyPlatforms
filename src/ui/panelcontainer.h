#ifndef PANELCONTAINER_H
#define PANELCONTAINER_H

#include "uiwidget.h"
#include "uipanel.h"

namespace game {

class PanelContainer : public UIWidget
{
public:
  PanelContainer(UIPanel &&panel, const Vector<float> &pos={}, const std::string &name="", bool hidden=false);

  void draw(Display &display, const Vector<float> &offset={}) override;

  Vector<float> getSize() const override;

  UIPanel &panel() { return mPanel; }

private:
  UIPanel mPanel;
};

} // namespace game

#endif // PANELCONTAINER_H
