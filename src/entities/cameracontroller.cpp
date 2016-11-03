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
  mBottomMargin(mTopMargin),
  mOriginalBaseY(mBaseY),
  mOriginalTopMargin(mTopMargin),
  mOriginalBottomMargin(mBottomMargin),
  mPanTarget(baseY),
  mPanInitialPos(baseY),
  mEntityCtrl(entity.getComponent<ActorControlComponent>()),
  mPhysics(*entity.getComponent<MovingPhysicsComponent>()),
  mCameraControlTrigger(Entity::none)
{
  GameState::current().getCamera().viewport().y = baseY;
}

void CameraAnchoredY::update(uint32_t step, Entity &entity, Camera &camera)
{
  Rect<float> &viewport = camera.viewport();
  const Rect<float> &box = entity.getGlobalBox();

  // Always follow on X
  viewport.x = (box.x + box.w/2.f) - viewport.w/2.f;

  mPanTarget = viewport.y;
  // If the hero is near the top of the viewport,
  // pan just high enough for it to see where a jump would lead
  if (mEntityCtrl && mPhysics.isOnGround()) {

    // The -5.f is for float inconsistencies
    float maxJump = box.y - box.h - mEntityCtrl->getMaxJumpHeight() - 5.f;
    mPanTarget = std::min(mBaseY, maxJump);
    mPanInitialPos = viewport.y;
  }

  // Always keep the hero in sight, and apply a margin
  mPanTarget = std::min(mPanTarget, box.y - mTopMargin);
  mPanTarget = std::max(mPanTarget, box.y + box.h + mBottomMargin - viewport.h);

  // Move the camera
  float entityVelY = mPhysics.velocity().y;
  if (mPanTarget < viewport.y /*&& mPanTarget < mPanInitialPos*/) {

    float panVelocity = -400.f + ((entityVelY < 0.f) ? entityVelY : 0.f);
    viewport.y = std::max(mPanTarget, viewport.y + (panVelocity * step/1000.f));
  }
  else if (mPanTarget > viewport.y /*&& mPanTarget > mPanInitialPos*/) {

    float panVelocity = 400.f + ((entityVelY > 0.f) ? entityVelY : 0.f);
    viewport.y = std::min(mPanTarget, viewport.y + (panVelocity * step/1000.f));
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
    if (camMsg.bottomMargin >= 0.f) {
      mOriginalBottomMargin = mBottomMargin;
      mBottomMargin = camMsg.bottomMargin * Tile::SIZE;
    }
  }
}

} // namespace game
