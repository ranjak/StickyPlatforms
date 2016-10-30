#ifndef CAMERA_H
#define CAMERA_H

#include "entity.h"
#include "gamevector.h"
#include "rect.h"
#include <cstdint>

namespace game {


class Camera
{
public:
  Camera(float x, float y, float w, float h);

  const Rect<float> &getViewport() const;
  Rect<float> &viewport();

  Vector<float> toCamCoords(const Vector<float>& levelCoords) const;

  template<typename T>
  Rect<T> toCamCoords(Rect<T> levelCoords) const;

private:
  Rect<float> mViewport;
};

}

#endif // CAMERA_H
