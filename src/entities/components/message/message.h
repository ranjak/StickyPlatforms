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

  Message(Type type) : type(type) {}

  virtual ~Message() = 0;

  Type type;
};

inline Message::~Message() {}

} // namespace game

#endif // MESSAGE_H
