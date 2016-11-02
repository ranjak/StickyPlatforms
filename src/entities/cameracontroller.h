#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <cstdint>

namespace game {

class Entity;
class Camera;

class CameraController
{
public:
  virtual void update(std::uint32_t step, Entity &entity, Camera &camera) = 0;
};


class CameraFollowSimple : public CameraController
{
public:
  void update(uint32_t step, Entity &entity, Camera &camera) override;
};

} // namespace game

#endif // CAMERACONTROLLER_H
