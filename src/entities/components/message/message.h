#ifndef MESSAGE_H
#define MESSAGE_H

namespace game {

struct Message
{
  enum Type
  {
    DirectionChanged,
    OnCollision,
    ChildRemoved,
    Damage
  };

  Message(Type type) : type(type), handled(false) {}

  virtual ~Message() = 0;

  Type type;
  // Could be used by components that have hierarchies, like actor states,
  // to indicate whether a message should be propagated
  bool handled;

  void accept() { handled = true; }
};

inline Message::~Message() {}

} // namespace game

#endif // MESSAGE_H
