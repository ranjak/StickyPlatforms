#ifndef HEROSTATE_H
#define HEROSTATE_H

#include <cstdint>

namespace game {

class PlayerInputComponent;
class GameState;
class Message;

/**
 * @brief The current state the hero is in.
 * The hero always has to be in a single state.
 * States have their own way of handling input, physics and graphics.
 */
class HeroState
{
public:
    HeroState(PlayerInputComponent& stateMachine);

    virtual ~HeroState() = 0;

    virtual void update(std::uint32_t step, GameState &game);

    virtual void enter();

    virtual void receiveMessage(Message &msg);

protected:
    PlayerInputComponent& mStateMachine;
};

inline HeroState::~HeroState() {}

}

#endif // HEROSTATE_H
