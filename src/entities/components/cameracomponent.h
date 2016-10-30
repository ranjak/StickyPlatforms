#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "component.h"

namespace game {

class Entity;
class Camera;

class CameraComponent : public Component
{
public:
  CameraComponent(Entity &entity);

  ~CameraComponent();

  void update(std::uint32_t step, GameState &game) override;

  /**
   * @brief setActive Make this component the active camera controller.
   * The camera will then be attached to the entity that owns this component.
   */
  void setActive();

private:
  // The camera can't track more than one entity at a time!
  static CameraComponent *activeInstance;

  Entity &mEntity;
  Camera &mCamera;
};

} // namespace game

#endif // CAMERACOMPONENT_H
