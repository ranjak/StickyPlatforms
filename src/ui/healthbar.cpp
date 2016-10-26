#include "healthbar.h"
#include "color.h"
#include "healthcomponent.h"
#include "graphics.h"

namespace game {

HealthBar::HealthBar(const Rect<float> &bounds, const std::string &name, const Vector<float> &padding) :
  UIWidget(bounds, name),
  mPadding(padding),
  mHealthBar(0, 0, Color::GREEN),
  mFrame(0, 0, Color::WHITE)
{

}

void HealthBar::draw(Display &display) const
{
  mFrame.draw(display, mBounds);

  mHealthBar.draw(display, mBounds.x+mPadding.x, mBounds.y+mPadding.y);
}

void HealthBar::setSize(float w, float h)
{
  float ratio = w / mBounds.w;

  mHealthBar.setSize(ratio * mHealthBar.getSize().x, h - mPadding.y*2.f);
}

void HealthBar::setHealth(int health, int maxHealth)
{
  mHealthBar.setSize((mBounds.w - mPadding.x*2.f) * (static_cast<float>(health)/maxHealth), mBounds.h - mPadding.y*2.f);
}

} // namespace game
