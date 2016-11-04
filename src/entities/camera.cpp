#include "camera.h"
#include "game.h"
#include "entity.h"
#include "world/level.h"
#include "util.h"
#include <algorithm>

namespace game {


Camera::Camera(float x, float y, float w, float h) :
  mViewport(x, y, w, h)
{

}

const Rect<float> &Camera::getViewport() const
{
  return mViewport;
}

Rect<float> &Camera::viewport()
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
