#include "camera.h"
#include "gamestate.h"
#include "entity.h"
#include "world/level.h"
#include "util.h"
#include <algorithm>

namespace game {


Camera::Camera(float x, float y, float w, float h) :
  mViewport(x, y, w, h)
{

}

void Camera::update(uint32_t step, GameState &game)
{
  Level& level = game.getLevel();
  Vector<float> size = level.getPixelSize();

  // Track the hero, keep him in the middle.
  Entity *hero = level.getHero();

  if (hero) {
    Rect<float> box = hero->getGlobalBox();
    mViewport.setCenter(box.getCenter());

    // Do not go beyond the level's boundaries (but handle cases where the camera is larger than the level)
    mViewport.x = clamp(0.f, mViewport.x, std::max(size.x - mViewport.w, 0.f));
    mViewport.y = clamp(0.f, mViewport.y, std::max(size.y - mViewport.h, 0.f));
  }
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
Rect<T> Camera::toCamCoords(Rect<T> levelCoords) const
{
  levelCoords.x -= static_cast<T>(mViewport.x);
  levelCoords.y -= static_cast<T>(mViewport.y);
  return levelCoords;
}


template Rect<int> Camera::toCamCoords(Rect<int> levelCoords) const;
template Rect<float> Camera::toCamCoords(Rect<float> levelCoords) const;

}
