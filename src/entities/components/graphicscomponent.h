#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

#include "component.h"
#include "graphics.h"
#include "entity.h"
#include <memory>
#include <cstdint>

namespace game {

class Display;
class ActorControlComponent;

class GraphicsComponent : public Component
{
public:
  GraphicsComponent(std::unique_ptr<Graphics> graphics, ActorControlComponent *controller=nullptr);


  void draw(Display &display, const Vector<float> &pos);

  void setVisible(bool visible);

  /**
   * @brief setBlinking Make the graphics blink (rapidly toggle between visible/invisible).
   * @param duration Duration in seconds of the effect.
   * @param period How long should a blinking "state" (visible or invisible) last, in milliseconds.
   */
  void setBlinking(float duration, std::uint32_t period=10);

  void setDirection(int direction);

private:
  void updateDelegate(std::uint32_t step, Game &game);

private:
  std::unique_ptr<Graphics> mGraphics;
  bool mVisible;

  int mNumBlinkSwitches;
  std::uint32_t mBlinkPeriod;
  std::uint32_t mNextBlinkSwitch;

  ActorControlComponent *mController;
};

} // namespace game

#endif // GRAPHICSCOMPONENT_H
