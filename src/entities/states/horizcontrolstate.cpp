#include "horizcontrolstate.h"
#include "inputcomponent.h"
#include "gamecommands.h"
#include "gamevector.h"
#include "actorcontrolcomponent.h"
#include "physicscomponent.h"
#include <cmath>
#include <algorithm>

namespace game {


HorizControlState::HorizControlState(ActorControlComponent &stateMachine, float acceleration, float maxSpeed) :
  ActorState(stateMachine),
  mDirection(0),
  mAcceleration(acceleration),
  mMaxSpeed(maxSpeed)
{

}

void HorizControlState::update(std::uint32_t step, GameState &game)
{
  int inputDirection = 0;

  if (mStateMachine.input().isHeld(Command::LEFT))
    inputDirection -= 1;

  if (mStateMachine.input().isHeld(Command::RIGHT))
    inputDirection += 1;

  Vector<float>& velocity = mStateMachine.physics().velocity();

  float accelAmount = mAcceleration * step / 1000.f;

  // Accelerate until max speed
  if (inputDirection < 0 && velocity.x > - mMaxSpeed)
    velocity.x = std::max(-mMaxSpeed, velocity.x - accelAmount);

  else if (inputDirection > 0 && velocity.x < mMaxSpeed)
    velocity.x = std::min(mMaxSpeed, velocity.x + accelAmount);

  // No direction : decelerate until stop
  else if (velocity.x > 0)
    velocity.x = std::max(0.f, velocity.x - accelAmount);
  else if (velocity.x < 0)
    velocity.x = std::min(0.f, velocity.x + accelAmount);

  mDirection = inputDirection;

//  if (game.getCommands().isHit(Command::SWORD))
  //    mHero.swingSword();
}

}
