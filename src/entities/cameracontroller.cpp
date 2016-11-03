#include "cameracontroller.h"
#include "rect.h"
#include "entity.h"
#include "camera.h"
#include "gamevector.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "gamestate.h"
#include "world/tile.h"
#include "triggermsg.h"
#include "cameracontrolmsg.h"

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
  mTopMargin(entity.getGlobalBox().h),
  mOriginalBaseY(baseY),
  mOriginalTopMargin(mTopMargin),
  mPanTarget(baseY),
  mPanInitialPos(baseY),
  mEntityCtrl(entity.getComponent<ActorControlComponent>()),
  mPhysics(*entity.getComponent<MovingPhysicsComponent>()),
  mCameraControlTrigger(Entity::none)
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

    // The -5.f is for float inconsistencies
    float maxJump = box.y - box.h - mEntityCtrl->getMaxJumpHeight() - 5.f;
    mPanTarget = std::min(mBaseY, maxJump);
    mPanInitialPos = viewport.y;
  }

  if (mPanTarget < viewport.y && mPanTarget < mPanInitialPos)
    viewport.y = std::max(mPanTarget, viewport.y - 400.f*step/1000.f);
  else if (mPanTarget > viewport.y && mPanTarget > mPanInitialPos)
    viewport.y = std::min(mPanTarget, viewport.y + 400.f*step/1000.f);

  // Always keep the hero in sight, and apply a margin
  if (box.y - mTopMargin < viewport.y) {
    viewport.y = box.y - mTopMargin;
    mPanTarget = viewport.y;
  }
  else if (box.y + 2.f*box.h > viewport.y + viewport.h) {
    viewport.y = box.y + 2.f*box.h - viewport.h;
    mPanTarget = viewport.y;
  }
}

void CameraAnchoredY::receiveMessage(Message &msg)
{
  if (msg.type != Message::TriggerEnter && msg.type != Message::TriggerExit)
    return;

  TriggerMsg &trg = static_cast<TriggerMsg &>(msg);

  if (msg.type == Message::TriggerExit && trg.triggerEntity == mCameraControlTrigger) {

    mBaseY = mOriginalBaseY;
    mTopMargin = mOriginalTopMargin;
  }
  else if (msg.type == Message::TriggerEnter && trg.msg->type == Message::CameraControl) {

    mCameraControlTrigger = trg.triggerEntity;

    CameraControlMsg &camMsg = static_cast<CameraControlMsg &>(*trg.msg);

    if (camMsg.baseY >= 0.f) {
      mOriginalBaseY = mBaseY;
      mBaseY = camMsg.baseY * Tile::SIZE;
    }
    if (camMsg.topMargin >= 0.f) {
      mOriginalTopMargin = mTopMargin;
      mTopMargin = camMsg.topMargin * Tile::SIZE;
    }
  }
}

} // namespace game
