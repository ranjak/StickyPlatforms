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


HorizControlState::HorizControlState(ActorControlComponent &stateMachine, float acceleration, float friction, float maxSpeed) :
  ActorState(stateMachine),
  mAcceleration(acceleration),
  mFriction(friction),
  mMaxSpeed(maxSpeed)
{

}

void HorizControlState::update(std::uint32_t step, Game &game)
{
  int inputDirection = 0;

  if (mStateMachine.input().isHeld(Command::LEFT))
    inputDirection -= 1;

  if (mStateMachine.input().isHeld(Command::RIGHT))
    inputDirection += 1;

  Vector<float>& velocity = mStateMachine.physics().velocity();

  switch (inputDirection) {

  case -1:
    velocity.x = std::max(-mMaxSpeed, velocity.x - mAcceleration * step / 1000.f);
    break;

  case 1:
    velocity.x = std::min(mMaxSpeed, velocity.x + mAcceleration * step / 1000.f);
    break;

  case 0:
    if (velocity.x < 0)
      velocity.x = std::min(0.f, velocity.x + mFriction * step / 1000.f);
    else
      velocity.x = std::max(0.f, velocity.x - mFriction * step / 1000.f);
    break;
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
