#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "component.h"
#include "gamevector.h"

namespace game {

class Entity;
class Camera;

class CameraComponent : public Component
{
public:
  CameraComponent(Entity &entity);

  ~CameraComponent();

  void update(std::uint32_t step, GameState &game) override;

  void receiveMessage(Message &msg) override;

  /**
   * @brief setActive Make this component the active camera controller.
   * The camera will then be attached to the entity that owns this component.
   */
  void setActive();

  void shake(float duration, float radius=15.f);

  void updateShake();

private:
  // The camera can't track more than one entity at a time!
  static CameraComponent *activeInstance;

  Entity &mEntity;
  Camera &mCamera;
  Vector<float> mVelocity;

  float mShakeRadius;
  float mShakeAngle;
  std::uint32_t mShakeEndTimestamp;
};

} // namespace game

#endif // CAMERACOMPONENT_H
