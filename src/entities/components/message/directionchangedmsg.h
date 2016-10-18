#ifndef DIRECTIONCHANGEDMSG_H
#define DIRECTIONCHANGEDMSG_H

#include "message.h"

namespace game {

struct DirectionChangedMsg : public Message
{
  DirectionChangedMsg(int direction) : Message(DirectionChanged), direction(direction) {}

  const int direction;
};

} // namespace game

#endif // DIRECTIONCHANGEDMSG_H
