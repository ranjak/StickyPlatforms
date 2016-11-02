#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "gamevector.h"
#include <cstdint>

namespace game {

class Entity;
class Camera;
class ActorControlComponent;
class MovingPhysicsComponent;

class CameraController
{
public:
  virtual void update(std::uint32_t step, Entity &entity, Camera &camera) = 0;
};

/**
 * @brief Keep the hero at the center of the screen at all times.
 */
class CameraFollowSimple : public CameraController
{
public:
  void update(uint32_t step, Entity &entity, Camera &camera) override;
};

/**
 * @brief Keep the hero centered on the X axis, but limit movement on the Y axis.
 * The camera's Y coordinate is fixed at a specific value,
 * and only varies when the hero is near the top or bottom of the viewport with a panning motion,
 * while always trying to go back to that specific Y value.
 * Said Y value can be set by special triggers in levels.
 */
class CameraAnchoredY : public CameraController
{
public:
  CameraAnchoredY(Entity &entity);
  CameraAnchoredY(Entity &entity, float baseY);

  void update(uint32_t step, Entity &entity, Camera &camera) override;

private:
  // Y coordinate the camera is anchored to.
  float mBaseY;
  // Y velocity when panning.
  float mVelocity;

  float mPanTarget;
  float mPanInitialPos;

  ActorControlComponent *mEntityCtrl;
  MovingPhysicsComponent &mPhysics;
};

} // namespace game

#endif // CAMERACONTROLLER_H
