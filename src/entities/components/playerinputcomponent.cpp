#include "playerinputcomponent.h"
#include "airstate.h"

namespace game {

PlayerInputComponent::PlayerInputComponent(Entity &player, WalkComponent &walkComp) :
  mPlayer(player),
  mState(),
  mNextState(new AirState(*this, walkComp))
{

}

void PlayerInputComponent::update(uint32_t step, GameState &game)
{
  if (mNextState) {
    mState = std::move(mNextState);
    mState->enter();
  }

  mState->update(step, game);
}

void PlayerInputComponent::receiveMessage(Message &msg)
{
  mState->receiveMessage(msg);
}

void PlayerInputComponent::setState(std::unique_ptr<HeroState> newState)
{
  mNextState = std::move(newState);
}

} // namespace game
