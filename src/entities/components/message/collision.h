#ifndef COLLISION_H
#define COLLISION_H

#include "message.h"
#include "entity.h"
#include "gamevector.h"
#include "rect.h"

namespace game {

struct Collision : public Message
{
  Collision(EntityID entity, const Rect<float> &bbox, const Vector<int> &normal, bool isObstacle) : Message(OnCollision), entity(entity), bbox(bbox), normal(normal), isObstacle(isObstacle) {}

  /** ID of the colliding entity, or Entity::none if this is a tile collision */
  EntityID entity;

  Rect<float> bbox;

  Vector<int> normal;

  bool isObstacle;
};

}

#endif // COLLISION_H
