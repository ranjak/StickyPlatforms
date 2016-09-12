#include "platform.h"
#include "camera.h"

namespace game {


Platform::Platform() :
  Platform(0, 0)
{

}

Platform::Platform(int x, int y, int w, int h) :
  Entity(x, y, w, h),
  mGraphics(w, h)
{

}

// Our platforms are purely static, they do't need updating.
void Platform::update(uint32_t step, GameState &game)
{

}

// FIXME duplicate code detected! See Hero.
void Platform::draw(Display &target, const Camera &cam) const
{
  Vector<float> pos = cam.toCamCoords(getPosition());

  mGraphics.draw(target, pos.x, pos.y);
}

} // namespace game
