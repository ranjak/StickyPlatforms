#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "gamevector.h"
#include "entity.h"
#include <cstdint>

namespace game {

class Camera;
class ActorControlComponent;
class MovingPhysicsComponent;
class Message;

class CameraController
{
public:
  virtual void update(std::uint32_t step, Entity &entity, Camera &camera) = 0;

  virtual void receiveMessage(Message &msg) {}
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

  void receiveMessage(Message &msg) override;

  void setBaseY(float base);
  void setTopMargin(float tileMargin);

private:
  // Y coordinate the camera is anchored to.
  float mBaseY;
  // Minimum number of pixels to keep between the hero and the top/bottom of the viewport.
  float mTopMargin;
  float mBottomMargin;
  // These 2 values can be changed by a trigger. Keep the original ones so we can restore them.
  float mOriginalBaseY, mOriginalTopMargin, mOriginalBottomMargin;

  float mPanTarget;
  float mPanInitialPos;

  ActorControlComponent *mEntityCtrl;
  MovingPhysicsComponent &mPhysics;
  // This value is set when the entity is inside a camera control trigger
  EntityID mCameraControlTrigger;
};

} // namespace game

#endif // CAMERACONTROLLER_H
