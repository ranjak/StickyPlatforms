#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "component.h"
#include "gamevector.h"
#include "cameracontroller.h"
#include "make_unique.h"
#include <memory>

namespace game {

class Entity;
class Camera;

class CameraComponent : public Component
{
public:
  CameraComponent(Entity &entity, std::unique_ptr<CameraController> controller=std::make_unique<CameraFollowSimple>());

  ~CameraComponent();

  /**
   * @brief setActive Make this component the active camera controller.
   * The camera will then be attached to the entity that owns this component.
   */
  void setActive();

  void shake(float duration, float radius=15.f);

  void updateShake();

private:
  void updateDelegate(std::uint32_t step, Game &game) override;

  void receiveMessageDelegate(Message &msg) override;

private:
  // The camera can't track more than one entity at a time!
  static CameraComponent *activeInstance;

  Entity &mEntity;
  Camera &mCamera;
  // Delegate object that implements custom logic to track the entity
  std::unique_ptr<CameraController> mController;

  float mShakeRadius;
  float mShakeAngle;
  std::uint32_t mShakeEndTimestamp;
};

} // namespace game

#endif // CAMERACOMPONENT_H
