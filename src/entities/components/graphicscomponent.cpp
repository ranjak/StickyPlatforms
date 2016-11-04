#include "graphicscomponent.h"
#include "game.h"
#include <cassert>

namespace game {

GraphicsComponent::GraphicsComponent(std::unique_ptr<Graphics> graphics) :
  mGraphics(std::move(graphics)),
  mVisible(true),
  mNumBlinkSwitches(0),
  mBlinkPeriod(0),
  mNextBlinkSwitch(0)
{

}

void GraphicsComponent::updateDelegate(uint32_t step, Game &game)
{
  if (mNumBlinkSwitches > 0 && mNextBlinkSwitch <= game.now()) {
    mVisible = !mVisible;

    if (--mNumBlinkSwitches == 0)
      mVisible = true;
    else
      mNextBlinkSwitch = game.now() + mBlinkPeriod;
  }
}

void GraphicsComponent::draw(Display &display, const Vector<float> &pos) const
{
  if (mVisible)
    mGraphics->draw(display, Game::current().getCamera().toCamCoords(pos));
}

void GraphicsComponent::setVisible(bool visible)
{
  // Cancel blinking
  if (!visible)
    mNumBlinkSwitches = 0;

  mVisible = visible;
}

void GraphicsComponent::setBlinking(float duration, std::uint32_t period)
{
  assert(duration > 0);

  if (!mVisible)
    return;

  mBlinkPeriod = period;
  mNextBlinkSwitch = Game::current().now() + period;
  mNumBlinkSwitches = static_cast<int>((duration * 1000.f) / period);

  mVisible = false;
}

} // namespace game
