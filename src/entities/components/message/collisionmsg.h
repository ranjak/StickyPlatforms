#ifndef COLLISIONMSG_H
#define COLLISIONMSG_H

#include "message.h"
#include "entity.h"
#include "gamevector.h"

namespace game {

struct CollisionMsg : public Message
{
  CollisionMsg(EntityID entity, const Vector<int> &normal, bool isObstacle) : Message(Collision), entity(entity), tilePos(), normal(normal), isObstacle(isObstacle) {}

  CollisionMsg(const Vector<int> &tilePos, const Vector<int> &normal, bool isObstacle) : Message(Collision), entity(Entity::none), tilePos(tilePos), normal(normal), isObstacle(isObstacle) {}

  /** ID of the colliding entity, or Entity::none if this is a tile collision */
  const EntityID entity;

  /** If this collision is with a tile, this will contain its position in pixels */
  const Vector<int> tilePos;

  const Vector<int> normal;

  bool isObstacle;
};

}

#endif // COLLISIONMSG_H
