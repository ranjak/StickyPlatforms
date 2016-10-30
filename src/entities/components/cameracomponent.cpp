#include "cameracomponent.h"
#include "camera.h"
#include "world/level.h"
#include "gamestate.h"
#include "entity.h"
#include "util.h"
#include "rect.h"
#include <algorithm>

namespace game {

CameraComponent *CameraComponent::activeInstance = nullptr;

CameraComponent::CameraComponent(Entity &entity) :
  mEntity(entity),
  mCamera(GameState::current().getCamera())
{
  if (!activeInstance)
    activeInstance = this;
}

CameraComponent::~CameraComponent()
{
  if (this == activeInstance)
    activeInstance = nullptr;
}

void CameraComponent::update(std::uint32_t step, GameState &game)
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
}

void CameraComponent::setActive()
{
  activeInstance = this;
}

} // namespace game
