#include "clingstate.h"
#include "gamestate.h"
#include "gravitycomponent.h"
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

  GravityComponent *gravity = mStateMachine.entity().getComponent<GravityComponent>();
  if (gravity)
    gravity->setEnabled(false);
}

void ClingState::update(uint32_t step, GameState &game)
{
  HorizControlState::update(step, game);

  if (!canStillCling(game)) {
    mStateMachine.setState(ActorControlComponent::AIR);
  }
}

void ClingState::exit()
{
  GravityComponent *gravity = mStateMachine.entity().getComponent<GravityComponent>();
  if (gravity)
    gravity->setEnabled(true);
}

bool ClingState::canStillCling(GameState &game)
{
  if (game.now() >= mClingTimeout || mStateMachine.input().isReleased(Command::JUMP))
    return false;

  // Make sure we're still in contact with a tile above us
  const std::vector<std::pair<Vector<int>, Vector<int>>> &tileCollisions = mStateMachine.physics().getCollidingTiles();

  for (const std::pair<Vector<int>,Vector<int>> &col : tileCollisions) {
    if (game.getLevel().getTileAt(col.first).isObstacle() && col.second.y > 0)
      return true;
  }

  return false;
}

} // namespace game
