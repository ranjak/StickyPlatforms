#ifndef CAMERA_H
#define CAMERA_H

#include "entity.h"
#include "gamevector.h"

namespace game {


class Camera : public Entity
{
public:
  Camera();
  Camera(int x, int y, int w, int h);

  Vector<float> toCamCoords(const Vector<float>& levelCoords) const;

};

}

#endif // CAMERA_H
