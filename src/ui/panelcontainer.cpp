#include "panelcontainer.h"

namespace game {


PanelContainer::PanelContainer(UIPanel &&panel, const Vector<float> &pos, const std::string &name, bool hidden) :
  UIWidget(pos, name, hidden),
  mPanel(std::move(panel))
{

}

void PanelContainer::draw(Display &display, const Vector<float> &offset)
{
  mPanel.draw(display, offset + mPos);
}

Vector<float> PanelContainer::getSize() const
{
  return mPanel.getSize();
}

} // namespace game
