#include "climbplatformstate.h"
#include "movingphysicscomponent.h"
#include "collision.h"
#include "actorcontrolcomponent.h"
#include "gamestate.h"
#include "world/level.h"
#include "world/tile.h"

namespace game {


ClimbPlatformState::ClimbPlatformState(ActorControlComponent &stateMachine) :
  ActorState(stateMachine),
  mPlatform(),
  mEntityObstacles(),
  mTileObstacles()
{

}

void ClimbPlatformState::enter()
{
  MovingPhysicsComponent &physics = mStateMachine.physics();
  const Level &level = GameState::current().getLevel();

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

void ClimbPlatformState::update(uint32_t step, GameState &game)
{
  // Over the platform ?
  const Rect<float> &box = mStateMachine.entity().getGlobalBox();

  if (box.y + box.h <= mPlatform.y) {
    mStateMachine.setState(ActorControlComponent::AIR_CLINGABLE);
  }
}

void ClimbPlatformState::exit()
{
  MovingPhysicsComponent &physics = mStateMachine.physics();

  physics.setGravityEnabled(true);
  physics.velocity().y = 0.f;

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

} // namespace game
