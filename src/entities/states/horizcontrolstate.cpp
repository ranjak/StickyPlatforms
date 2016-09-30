#include "horizcontrolstate.h"
#include "gamecommands.h"
#include "gamevector.h"
#include "gamestate.h"
#include "hero.h"
#include "playerinputcomponent.h"
#include "directionchangedmsg.h"
#include <cmath>
#include <algorithm>

namespace game {


HorizControlState::HorizControlState(PlayerInputComponent &stateMachine, WalkComponent &walkComp, float acceleration, float maxSpeed) :
  HeroState(stateMachine),
  mWalkComp(walkComp),
  mDirection(0),
  mAcceleration(acceleration),
  mMaxSpeed(maxSpeed)
{

}

void HorizControlState::update(std::uint32_t step, GameState &game)
{
  int inputDirection = 0;

  if (game.getCommands().isHeld(Command::LEFT))
    inputDirection -= 1;

  if (game.getCommands().isHeld(Command::RIGHT))
    inputDirection += 1;

  if (inputDirection != mDirection) {
    mWalkComp.setDirection(inputDirection);
    mDirection = inputDirection;
//    mStateMachine.getPlayer().sendMessage(std::unique_ptr<DirectionChangedMsg>(new DirectionChangedMsg(inputDirection)));
  }

//  if (game.getCommands().isHit(Command::SWORD))
  //    mHero.swingSword();
}

void HorizControlState::enter()
{
  mWalkComp.setMaxSpeed(mMaxSpeed);
  mWalkComp.setAcceleration(mAcceleration);
}

}
