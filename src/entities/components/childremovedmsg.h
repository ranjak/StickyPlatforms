#ifndef CHILDREMOVED_H
#define CHILDREMOVED_H

#include "message.h"
#include "entity.h"

namespace game {

struct ChildRemovedMsg : public Message
{
  ChildRemovedMsg(EntityID childId) : Message(ChildRemoved), id(childId) {}

  EntityID id;
};

} // namespace game

#endif // CHILDREMOVED_H
