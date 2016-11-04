#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>

namespace game {

class Entity;
class Game;
struct Message;

class Component
{
public:
  Component() : mEnabled(true) {}

  virtual ~Component() = 0;

  void setEnabled(bool enable) { mEnabled = enable; }

  void update(std::uint32_t step, Game &game)
  {
    if (mEnabled)
      updateDelegate(step, game);
  }

  void receiveMessage(Message &msg)
  {
    if (mEnabled)
      receiveMessageDelegate(msg);
  }

private:
  virtual void updateDelegate(std::uint32_t step, Game &game) {}

  virtual void receiveMessageDelegate(Message &msg) {}

private:
  bool mEnabled;
};

inline Component::~Component() {}

}

#endif // COMPONENT_H
