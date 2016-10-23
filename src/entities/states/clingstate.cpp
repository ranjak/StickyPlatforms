#include "clingstate.h"
#include "gamestate.h"
#include "actorcontrolcomponent.h"
#include "inputcomponent.h"
#include "movingphysicscomponent.h"
#include "world/level.h"
#include "collision.h"
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
    const std::vector<Collision> &collisions = mStateMachine.physics().getCollisions();

    // Find the highest obstacle we need to climb over
    // Remember, higher = lower y!
    float highestObstacleY = game.getLevel().getPixelSize().y;

    for (const Collision &col : collisions) {

      if (col.isObstacle && col.normal.y > 0)
        highestObstacleY = std::min(highestObstacleY, col.bbox.y);
    }

    // Make sure the tile to climb over is thin enough
    // also, there must not be any obstacle above the platform
    // TODO: also check entities here instead of just tiles
    if (box.y - highestObstacleY <= box.h / 2.f &&
        game.getLevel().getObstaclesInArea(Rect<float>(box.x, highestObstacleY-box.h, box.w, box.h), mStateMachine.physics()).empty())
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
  const std::vector<Collision> &collisions = mStateMachine.physics().getCollisions();

  for (const Collision &col : collisions) {
    if (col.isObstacle && col.normal.y > 0)
      return true;
  }

  return false;
}

} // namespace game
