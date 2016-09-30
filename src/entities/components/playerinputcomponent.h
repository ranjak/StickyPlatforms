#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H

#include "component.h"
#include "herostate.h"
#include <memory>

namespace game {

class WalkComponent;

class PlayerInputComponent : public Component
{
public:
  PlayerInputComponent(Entity &player, WalkComponent &walkComp);

  void update(std::uint32_t step, GameState &game) override;

  void receiveMessage(Message &msg) override;

  void setState(std::unique_ptr<HeroState> newState);

  Entity &getPlayer() { return mPlayer; }

private:
  Entity &mPlayer;
  std::unique_ptr<HeroState> mState;
  std::unique_ptr<HeroState> mNextState;
};

} // namespace game

#endif // PLAYERINPUTCOMPONENT_H
