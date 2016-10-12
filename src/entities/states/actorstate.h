#ifndef ACTORSTATE_H
#define ACTORSTATE_H

#include <cstdint>

namespace game {

class ActorControlComponent;
class GameState;
class Message;

/**
 * @brief The current state an actor is in.
 * Actors (active entities like the hero and enemies) always have to be in a single state.
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

    /**
     * @brief getDirection Get the actor's facing direction.
     * @return -1 if the actor is facing left, +1 if it's facing right.
     */
    virtual int getDirection();

protected:
    ActorControlComponent& mStateMachine;
};

inline ActorState::~ActorState() {}

}

#endif // ACTORSTATE_H
