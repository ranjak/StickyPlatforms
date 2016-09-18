#include "camera.h"
#include "gamestate.h"
#include "hero.h"
#include "world/level.h"
#include "util.h"

namespace game {

Camera::Camera() :
  Entity()
{

}

Camera::Camera(int x, int y, int w, int h) :
  Entity(x, y, w, h)
{

}

void Camera::update(uint32_t step, GameState &game)
{
  Level& level = game.getLevel();
  Vector<int> size = level.getPixelSize();

  // Track the hero, keep him in the middle.
  const Rect<float> &box = level.getHero()->getBoundingBox();
  mBoundingBox.setCenter(box.getCenter());

  // Do not go beyond the level's boundaries (but handle cases where the camera is larger than the level)
  mBoundingBox.x = clamp(0.f, mBoundingBox.x, std::max(size.x - mBoundingBox.w, 0.f));
  mBoundingBox.y = clamp(0.f, mBoundingBox.y, std::max(size.y - mBoundingBox.h, 0.f));
}

Vector<float> Camera::toCamCoords(const Vector<float> &levelCoords) const
{
  return Vector<float>(levelCoords.x - mBoundingBox.x, levelCoords.y - mBoundingBox.y);
}

Rect<float> Camera::toCamCoords(const Rect<float> &levelCoords) const
{
  return Rect<float>(levelCoords.x - mBoundingBox.x, levelCoords.y - mBoundingBox.y, levelCoords.w, levelCoords.h);
}

}
