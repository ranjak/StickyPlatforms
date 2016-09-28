#ifndef CAMERA_H
#define CAMERA_H

#include "entity.h"
#include "gamevector.h"
#include <cstdint>

namespace game {

template<typename T> class Rect;

class Camera : public Entity
{
public:
  Camera();
  Camera(int x, int y, int w, int h);

  void update(std::uint32_t step, GameState &game) override;

  Vector<float> toCamCoords(const Vector<float>& levelCoords) const;

  template<typename T>
  Rect<T> toCamCoords(const Rect<T>& levelCoords) const;

};

}

#endif // CAMERA_H
