#include "graphicscomponent.h"
#include "game.h"
#include "herosquare.h"
#include "actorcontrolcomponent.h"
#include <cassert>

namespace game {

GraphicsComponent::GraphicsComponent(std::unique_ptr<Graphics> graphics, ActorControlComponent *controller) :
  mGraphics(std::move(graphics)),
  mVisible(true),
  mNumBlinkSwitches(0),
  mBlinkPeriod(0),
  mNextBlinkSwitch(0),
  mController(controller)
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

void GraphicsComponent::draw(Display &display, const Vector<float> &pos)
{
  // TODO make more generic if graphics get more fleshed out
  if (mController)
    setDirection(mController->getDirection());

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

void GraphicsComponent::setDirection(int direction)
{
  HeroSquare *hero = dynamic_cast<HeroSquare *>(mGraphics.get());

  if (hero)
    hero->setDirection(direction);
}

} // namespace game
