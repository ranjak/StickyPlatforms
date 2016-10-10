#include "camera.h"
#include "gamestate.h"
#include "entity.h"
#include "world/level.h"
#include "util.h"

namespace game {


Camera::Camera(int x, int y, int w, int h) :
  mViewport(x, y, w, h)
{

}

void Camera::update(uint32_t step, GameState &game)
{
  Level& level = game.getLevel();
  Vector<float> size = level.getPixelSize();

  // Track the hero, keep him in the middle.
  Rect<float> box = level.getHero()->getGlobalBox();
  mViewport.setCenter(box.getCenter());

  // Do not go beyond the level's boundaries (but handle cases where the camera is larger than the level)
  mViewport.x = clamp(0.f, mViewport.x, std::max(size.x - mViewport.w, 0.f));
  mViewport.y = clamp(0.f, mViewport.y, std::max(size.y - mViewport.h, 0.f));
}

const Rect<float> &Camera::getViewport() const
{
  return mViewport;
}

Vector<float> Camera::toCamCoords(const Vector<float> &levelCoords) const
{
  return Vector<float>(levelCoords.x - mViewport.x, levelCoords.y - mViewport.y);
}

template<typename T>
Rect<T> Camera::toCamCoords(const Rect<T> &levelCoords) const
{
  return Rect<T>(levelCoords.x - mViewport.x, levelCoords.y - mViewport.y, levelCoords.w, levelCoords.h);
}


template Rect<int> Camera::toCamCoords(const Rect<int> &levelCoords) const;
template Rect<float> Camera::toCamCoords(const Rect<float> &levelCoords) const;

}
