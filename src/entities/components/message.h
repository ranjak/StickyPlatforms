#ifndef MESSAGE_H
#define MESSAGE_H

namespace game {

struct Message
{
  enum Type
  {
    DirectionChanged,
    ObstacleReached
  };

  Message(Type type) : type(type) {}

  virtual ~Message() = 0;

  const Type type;
};

inline Message::~Message() {}

} // namespace game

#endif // MESSAGE_H
