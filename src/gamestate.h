#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <cstdint>

namespace game {

/**
 * @brief A particular state the running game can be in.
 * For instance: playing, paused, loading...
 */
template<typename... Args>
class GameState
{
public:

  virtual ~GameState() = 0;

  virtual void update(std::uint32_t step) {}

  virtual void enter(Args... args) {}

  virtual void exit() {}
};

template<typename... Args>
inline GameState<Args>::~GameState() {}

} // namespace game

#endif // GAMESTATE_H
