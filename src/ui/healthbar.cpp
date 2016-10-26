#include "healthbar.h"
#include "color.h"
#include "healthcomponent.h"
#include "graphics.h"
#include "gamevector.h"
#include "rect.h"

namespace game {

HealthBar::HealthBar(const Rect<float> &bounds, const std::string &name, const Vector<float> &padding) :
  UIWidget(Vector<float>(bounds.x, bounds.y), name),
  mPadding(padding),
  mHealthBar(0, 0, Color::GREEN),
  mFrame(static_cast<int>(bounds.w), static_cast<int>(bounds.h), Color::WHITE)
{

}

void HealthBar::draw(Display &display) const
{
  mFrame.draw(display, mPos);

  mHealthBar.draw(display, mPos.x+mPadding.x, mPos.y+mPadding.y);
}

void HealthBar::setSize(float w, float h)
{
  float ratio = w / mFrame.getSize().x;

  mHealthBar.setSize(ratio * mHealthBar.getSize().x, h - mPadding.y*2.f);

  mFrame.setSize(static_cast<int>(w), static_cast<int>(h));
}

Vector<float> HealthBar::getSize() const
{
  return mFrame.getSize();
}

void HealthBar::setHealth(int health, int maxHealth)
{
  mHealthBar.setSize((mFrame.getSize().x - mPadding.x*2.f) * (static_cast<float>(health)/maxHealth), mFrame.getSize().y - mPadding.y*2.f);
}

} // namespace game
