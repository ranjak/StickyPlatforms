#ifndef ACTORSTATE_H
#define ACTORSTATE_H

#include <cstdint>

namespace game {

class ActorControlComponent;
class GameState;
class Message;

/**
 * @brief The current state the hero is in.
 * The hero always has to be in a single state.
 * States have their own way of handling input, physics and graphics.
 */
class ActorState
{
public:
    ActorState(ActorControlComponent& stateMachine);

    virtual ~ActorState() = 0;

    virtual void update(std::uint32_t step, GameState &game);

    virtual void enter();

    virtual void receiveMessage(Message &msg);

protected:
    ActorControlComponent& mStateMachine;
};

inline ActorState::~ActorState() {}

}

#endif // ACTORSTATE_H
