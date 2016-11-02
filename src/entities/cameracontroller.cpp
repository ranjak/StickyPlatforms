#include "cameracontroller.h"
#include "rect.h"
#include "entity.h"
#include "camera.h"
#include "gamevector.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "gamestate.h"

namespace game {

void CameraFollowSimple::update(uint32_t step, Entity &entity, Camera &camera)
{
  camera.viewport().setCenter(entity.getGlobalBox().getCenter());
}

CameraAnchoredY::CameraAnchoredY(Entity &entity) :
  CameraAnchoredY(entity, entity.getGlobalPos().y - GameState::current().getCamera().getViewport().h/2.f)
{

}

CameraAnchoredY::CameraAnchoredY(Entity &entity, float baseY) :
  mBaseY(baseY),
  mVelocity(0.f),
  mPanTarget(baseY),
  mPanInitialPos(baseY),
  mEntityCtrl(entity.getComponent<ActorControlComponent>()),
  mPhysics(*entity.getComponent<MovingPhysicsComponent>())
{}

void CameraAnchoredY::update(uint32_t step, Entity &entity, Camera &camera)
{
  Rect<float> &viewport = camera.viewport();
  const Rect<float> &box = entity.getGlobalBox();

  // Always follow on X
  viewport.x = (box.x + box.w/2.f) - viewport.w/2.f;

  // If the hero is near the top of the viewport,
  // pan just high enough for it to see where a jump would lead it
  if (mEntityCtrl && mPhysics.isOnGround()) {

    float maxJump = box.y - mEntityCtrl->getMaxJumpHeight();
    mPanTarget = std::min(mBaseY, maxJump);
    mPanInitialPos = viewport.y;
  }

  if (mPanTarget < viewport.y && mPanTarget < mPanInitialPos)
    viewport.y = std::max(mPanTarget, viewport.y - 400.f*step/1000.f);
  else if (mPanTarget > viewport.y && mPanTarget > mPanInitialPos)
    viewport.y = std::min(mPanTarget, viewport.y + 400.f*step/1000.f);

  // Always keep the hero in sight, with a bottom margin equal to its height,
  // and a top margin equal to its jump height.
  if (box.y - box.h < viewport.y) {
    viewport.y = box.y - box.h;
    mPanTarget = viewport.y;
  }
  else if (box.y + 2.f*box.h > viewport.y + viewport.h) {
    viewport.y = box.y + 2.f*box.h - viewport.h;
    mPanTarget = viewport.y;
  }
}

} // namespace game
