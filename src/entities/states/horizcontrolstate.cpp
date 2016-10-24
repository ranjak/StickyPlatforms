#include "horizcontrolstate.h"
#include "inputcomponent.h"
#include "gamecommands.h"
#include "gamevector.h"
#include "actorcontrolcomponent.h"
#include "movingphysicscomponent.h"
#include "weaponcomponent.h"
#include <cmath>
#include <algorithm>

namespace game {


HorizControlState::HorizControlState(ActorControlComponent &stateMachine, float acceleration, float maxSpeed) :
  ActorState(stateMachine),
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

  float targetSpeed = mMaxSpeed * inputDirection;

  float accelAmount = mAcceleration * step / 1000.f;

  if (targetSpeed - velocity.x > 0) {
    velocity.x += std::min(accelAmount, targetSpeed-velocity.x);
  }
  else if (targetSpeed - velocity.x < 0) {
    velocity.x += std::max(-accelAmount, targetSpeed-velocity.x);
  }

  if (inputDirection != 0)
    mStateMachine.setDirection(inputDirection);

  if (mStateMachine.input().isHit(Command::SWORD)) {
    WeaponComponent *weap = mStateMachine.entity().getComponent<WeaponComponent>();
    if (weap)
      weap->swing(mStateMachine.getDirection());
  }
}

}
