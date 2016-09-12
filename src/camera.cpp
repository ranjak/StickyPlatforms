#include "camera.h"

namespace game {

Camera::Camera() :
  Entity()
{

}

Camera::Camera(int x, int y, int w, int h) :
  Entity(x, y, w, h)
{

}

Vector<float> Camera::toCamCoords(const Vector<float> &levelCoords) const
{
  return Vector<float>(levelCoords.x - mBoundingBox.x, levelCoords.y - mBoundingBox.y);
}

}
