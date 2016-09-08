#ifndef HEROSTATE_H
#define HEROSTATE_H

#include "gamestate.h"
#include "hero.h"
#include <cstdint>

namespace game {

class Hero;

/**
 * @brief The current state the hero is in.
 * The hero always has to be in a single state.
 * States have their own way of handling input, physics and graphics.
 */
class HeroState
{
public:
    HeroState(Hero& hero);

    virtual void update(std::uint32_t step, GameState &game);

    virtual void enter();

protected:
    Hero& mHero;
};

}

#endif // HEROSTATE_H
