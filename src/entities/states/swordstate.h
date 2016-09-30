#ifndef SWORDSTATE_H
#define SWORDSTATE_H

#include "herostate.h"
#include "sword.h"

namespace game {

class Display;
class Camera;
class Hero;

class SwordState : public HeroState
{
public:
  SwordState(Hero& hero);

  void update(std::uint32_t step, GameState &game) override;

  void enter() override;

  void draw(Display &display, const Camera &camera) const;

private:
  // When the state will end
  std::uint32_t mEndTimestamp;
  // When the sword will stop dealing damage
  std::uint32_t mDamageEndTimestamp;
  Sword mSword;
};

} // namespace game

#endif // SWORDSTATE_H
