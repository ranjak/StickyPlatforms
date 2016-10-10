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
  Camera(int x, int y, int w, int h);

  void update(std::uint32_t step, GameState &game);

  const Rect<float> &getViewport() const;

  Vector<float> toCamCoords(const Vector<float>& levelCoords) const;

  template<typename T>
  Rect<T> toCamCoords(const Rect<T>& levelCoords) const;

private:
  Rect<float> mViewport;
};

}

#endif // CAMERA_H
