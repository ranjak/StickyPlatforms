#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>

namespace game {

class Entity;
class GameState;
struct Message;

class Component
{
public:
  virtual ~Component() = 0;

  virtual void update(std::uint32_t step, GameState &game) {}

  virtual void receiveMessage(Message &msg) {}
};

inline Component::~Component() {}

}

#endif // COMPONENT_H
