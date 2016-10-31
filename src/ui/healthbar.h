#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "uiwidget.h"
#include "rectangle.h"
#include "frame.h"
#include "gamevector.h"
#include <string>

namespace game {

template<typename T> class Rect;

class HealthBar : public UIWidget
{
public:
  HealthBar(const Rect<float> &bounds, const std::string &name="health", const Vector<float> &padding=Vector<float>(2.f, 2.f));

  void draw(Display &display) override;

  void setSize(float w, float h) override;
  Vector<float> getSize() const override;

  void setHealth(int health, int maxHealth);

private:
  Vector<float> mPadding;
  Rectangle mHealthBar;
  Frame mFrame;
};

} // namespace game

#endif // HEALTHBAR_H
