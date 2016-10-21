#include "climbplatformstate.h"
#include "movingphysicscomponent.h"
#include "actorcontrolcomponent.h"
#include "gamestate.h"
#include "world/level.h"
#include "world/tile.h"

namespace game {


ClimbPlatformState::ClimbPlatformState(ActorControlComponent &stateMachine) :
  ActorState(stateMachine),
  mPlatform()
{

}

void ClimbPlatformState::enter()
{
  MovingPhysicsComponent &physics = mStateMachine.physics();
  const Level &level = GameState::current().getLevel();

  physics.setGravityEnabled(false);
  physics.setIgnoresObstacles(true);

  const std::vector<std::pair<Vector<int>, Vector<int>>> &tileCollisions = physics.getCollidingTiles();

  // Find the highest tile we need to climb over
  // Remember, higher = lower y!
  mPlatform.y = level.getPixelSize().y;

  for (const std::pair<Vector<int>,Vector<int>> &col : tileCollisions) {

    const Tile &tile = *level.getTileAt(col.first);

    if (tile.isObstacle() && col.second.y > 0 && tile.getCollisionBox(col.first).y < mPlatform.y)
      mPlatform = tile.getCollisionBox(col.first);
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
  physics.setIgnoresObstacles(false);
  physics.velocity().y = 0.f;
}

} // namespace game
