#include "climbplatformstate.h"
#include "movingphysicscomponent.h"
#include "collision.h"
#include "actorcontrolcomponent.h"
#include "game.h"
#include "world/level.h"
#include "world/tile.h"

namespace game {


ClimbPlatformState::ClimbPlatformState(ActorControlComponent &stateMachine) :
  ActorState(stateMachine),
  mPlatform(),
  mEntityObstacles(),
  mTileObstacles(),
  mFalling(false)
{

}

void ClimbPlatformState::enter()
{
  mFalling = false;
  MovingPhysicsComponent &physics = mStateMachine.physics();
  const Level &level = Game::current().getLevel();

  physics.setGravityEnabled(false);

  const std::vector<Collision> &collisions = physics.getCollisions();

  // Find the highest tile we need to climb over
  // Remember, higher = lower y!
  mPlatform.y = level.getPixelSize().y;

  // Ignore collisions with obstacles we need to pass through in order to climb
  for (const Collision &col : collisions) {

    if (col.isObstacle && col.normal.y > 0) {

      if (col.entity != Entity::none) {
        physics.setIgnored(col.entity, true);
        mEntityObstacles.push_back(col.entity);
      }
      else {
        Vector<int> tile(Tile::pixelToTile(col.bbox.x, col.bbox.y));
        physics.setIgnored(tile, true);
        mTileObstacles.push_back(tile);
      }

      if (col.bbox.y < mPlatform.y)
        mPlatform = col.bbox;
    }
  }

  physics.velocity().y = -400.f;
  physics.velocity().x = 0.f;
}

void ClimbPlatformState::update(uint32_t step, Game &game)
{
  const Rect<float> &box = mStateMachine.entity().getGlobalBox();

  // Over the platform ?
  if (box.y + box.h <= mPlatform.y) {
    mStateMachine.physics().velocity().y = 0.f;
    mStateMachine.setState(ActorControlComponent::AIR_CLINGABLE);
  }
  // Climbing interrupted -> fall through the platform
  else if (mFalling && box.y >= mPlatform.y+mPlatform.h) {
    mStateMachine.setState(ActorControlComponent::FALL);
  }
}

void ClimbPlatformState::exit()
{
  MovingPhysicsComponent &physics = mStateMachine.physics();

  physics.setGravityEnabled(true);

  // Re-enable collisions for ignored obstacles
  for (EntityID id : mEntityObstacles) {
    physics.setIgnored(id, false);
  }
  for (const Vector<int> &tile : mTileObstacles) {
    physics.setIgnored(tile, false);
  }

  mEntityObstacles.clear();
  mTileObstacles.clear();
}

void ClimbPlatformState::receiveMessage(Message &msg)
{
  // Fall through the platform when we hit an unexpected obstacle, or get hurt
  if (msg.type == Message::Damage ||
      (msg.type == Message::OnCollision && static_cast<Collision &>(msg).isObstacle && static_cast<Collision &>(msg).normal.y > 0))
  {
    mFalling = true;
    mStateMachine.physics().setGravityEnabled(true);
    mStateMachine.physics().velocity().y = 0.f;
    msg.accept();
  }

  if (!msg.handled)
    ActorState::receiveMessage(msg);
}

} // namespace game
