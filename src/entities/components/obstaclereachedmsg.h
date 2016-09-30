#ifndef OBSTACLEREACHEDMSG_H
#define OBSTACLEREACHEDMSG_H

#include "message.h"
#include "gamevector.h"

namespace game {

struct ObstacleReachedMsg : public Message
{
  ObstacleReachedMsg(const Vector<int> &normal) : Message(ObstacleReached), normal(normal) {}

  const Vector<int> normal;
};

}

#endif // OBSTACLEREACHEDMSG_H
