#include "cameracomponent.h"
#include "camera.h"
#include "world/level.h"
#include "gamestate.h"
#include "entity.h"
#include "util.h"
#include "rect.h"
#include "damagemsg.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

namespace game {

CameraComponent *CameraComponent::activeInstance = nullptr;

CameraComponent::CameraComponent(Entity &entity) :
  mEntity(entity),
  mCamera(GameState::current().getCamera()),
  mVelocity(),
  mShakeRadius(0.f),
  mShakeAngle(0.f),
  mShakeEndTimestamp(0)
{
  if (!activeInstance)
    activeInstance = this;
}

CameraComponent::~CameraComponent()
{
  if (this == activeInstance)
    activeInstance = nullptr;
}

void CameraComponent::updateDelegate(std::uint32_t step, GameState &game)
{
  if (this != activeInstance)
    return;

  Level& level = game.getLevel();
  Vector<float> size = level.getPixelSize();
  Rect<float> &viewport = mCamera.viewport();

  Rect<float> box = mEntity.getGlobalBox();
  viewport.setCenter(box.getCenter());

  // Do not go beyond the level's boundaries (but handle cases where the camera is larger than the level)
  viewport.x = clamp(0.f, viewport.x, std::max(size.x - viewport.w, 0.f));
  viewport.y = clamp(0.f, viewport.y, std::max(size.y - viewport.h, 0.f));

  if (mShakeEndTimestamp >= game.now())
    updateShake();
}

void CameraComponent::receiveMessageDelegate(Message &msg)
{
  if (msg.type == Message::Damage) {
    shake(0.25f, 8.f);
  }
}

void CameraComponent::setActive()
{
  activeInstance = this;
}

void CameraComponent::shake(float duration, float radius)
{
  mShakeEndTimestamp = GameState::current().now() + static_cast<std::uint32_t>(duration * 1000.f);
  mShakeRadius = radius;
}

void CameraComponent::updateShake()
{
  // Get a random angle roughly opposite to the current one
  mShakeAngle += 130.f + static_cast<float>(std::rand() % 100);

  float angleRads = toRad(mShakeAngle);
  mCamera.viewport().x += std::cos(angleRads)*mShakeRadius;
  mCamera.viewport().y += std::sin(angleRads)*mShakeRadius;

  mShakeRadius *= 0.9f;
}

} // namespace game
