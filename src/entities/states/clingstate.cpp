#include "clingstate.h"
#include "gamestate.h"
#include "actorcontrolcomponent.h"
#include "inputcomponent.h"
#include "movingphysicscomponent.h"
#include "world/level.h"
#include "world/tile.h"
#include "gamevector.h"
#include <vector>

namespace game {

ClingState::ClingState(ActorControlComponent &stateMachine) :
  HorizControlState(stateMachine, 2500.f, 200.f),
  mClingTimeout(0)
{

}

void ClingState::enter()
{
  mClingTimeout = GameState::current().now() + 2000;

  mStateMachine.physics().setGravityEnabled(false);
}

void ClingState::update(uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  if (!canStillCling(game)) {
    mStateMachine.setState(ActorControlComponent::AIR);
    return;
  }

  // Climb through thin platforms
  if (mStateMachine.input().isHeld(Command::UP)) {

    const Rect<float> &box = mStateMachine.entity().getGlobalBox();
    const std::vector<std::pair<Vector<int>, Vector<int>>> &tileCollisions = mStateMachine.physics().getCollidingTiles();

    // Find the highest tile we need to climb over
    // Remember, higher = lower y!
    float highestTileY = game.getLevel().getPixelSize().y;

    for (const std::pair<Vector<int>,Vector<int>> &col : tileCollisions) {

      Tile &tile = *game.getLevel().getTileAt(col.first);

      if (tile.isObstacle() && col.second.y > 0)
        highestTileY = std::min(highestTileY, tile.getCollisionBox(col.first).y);
    }

    // Make sure the tile to climb over is thin enough
    // also, there must not be any obstacle above the platform
    if (highestTileY - box.y <= box.h &&
        game.getLevel().getObstaclesInArea(Rect<float>(box.x, highestTileY-box.h, box.w, box.h)).empty())
    {
      mStateMachine.setState(ActorControlComponent::CLIMB_PLATFORM);
    }
  }
}

void ClingState::exit()
{
  mStateMachine.physics().setGravityEnabled(true);
}

bool ClingState::canStillCling(GameState &game)
{
  if (game.now() > mClingTimeout || mStateMachine.input().isReleased(Command::JUMP))
    return false;

  // Make sure we're still in contact with a tile above us
  const std::vector<std::pair<Vector<int>, Vector<int>>> &tileCollisions = mStateMachine.physics().getCollidingTiles();

  for (const std::pair<Vector<int>,Vector<int>> &col : tileCollisions) {
    if (game.getLevel().getTileAt(col.first)->isObstacle() && col.second.y > 0)
      return true;
  }

  return false;
}

} // namespace game
