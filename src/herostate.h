#ifndef HEROSTATE_H
#define HEROSTATE_H

namespace game {


/**
 * @brief The current state the hero is in.
 * The hero always has to be in a single state.
 * States have their own way of handling input, physics and graphics.
 */
class HeroState
{
public:
    HeroState();

    virtual void update();
};

}

#endif // HEROSTATE_H
