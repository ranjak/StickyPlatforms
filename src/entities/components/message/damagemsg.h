#ifndef DAMAGEMSG_H
#define DAMAGEMSG_H

#include "message.h"
#include "entitygroup.h"
#include "entity.h"

namespace game {

struct DamageMsg : public Message
{
public:
  DamageMsg(int points, EntityID source) : Message(Damage), points(points), source(source) {}

  int points;
  EntityID source;
};

} // namespace game

#endif // DAMAGEMSG_H
