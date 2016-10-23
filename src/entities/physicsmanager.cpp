#include "physicsmanager.h"
#include "gamevector.h"
#include "rect.h"
#include "entity.h"
#include "movingphysicscomponent.h"
#include "staticphysicscomponent.h"
#include "world/level.h"
#include <cassert>
#include <algorithm>

namespace game {

PhysicsManager::PhysicsManager(Level &level) :
  mMovingComps(),
  mStaticComps(),
  mLevel(level)
{

}

bool PhysicsManager::moveObject(MovingPhysicsComponent *object, const Vector<float> &dest)
{
  Entity &entity = object->entity();

  Rect<float> box = entity.getGlobalBox();
  Vector<float> direction(dest.x - box.x, dest.y - box.y);
  Vector<float> facingPoint;

  facingPoint.x = (direction.x >= 0) ? box.x + box.w : box.x;
  facingPoint.y = (direction.y >= 0) ? box.y + box.h : box.y;

  Vector<float> destFacingPoint(facingPoint.x + direction.x, facingPoint.y + direction.y);

  if (object->ignoresObstacles() || !object->isCollidable()) {

    box.x = (direction.x >= 0) ? destFacingPoint.x-box.w : destFacingPoint.x;
    box.y = (direction.y >= 0) ? destFacingPoint.y-box.h : destFacingPoint.y;

    entity.setGlobalPos(Vector<float>(box.x, box.y));
    return box.x == dest.x && box.y == dest.y;
  }

  // Retrieve possible obstacles in the movement area
  Rect<float> movementArea;
  movementArea.x = std::min(destFacingPoint.x, box.x);
  movementArea.y = std::min(destFacingPoint.y, box.y);
  movementArea.w = std::max(box.w, (movementArea.x == box.x) ? (destFacingPoint.x - box.x) : (box.x + box.w - destFacingPoint.x));
  movementArea.h = std::max(box.h, (movementArea.y == box.y) ? (destFacingPoint.y - box.y) : (box.y + box.h - destFacingPoint.y));

  std::vector<Rect<float>> obstacles = mLevel.getObstaclesInArea(movementArea, *object);

  std::for_each(mStaticComps.begin(), mStaticComps.end(), [&](StaticPhysicsComponent *p) {
    if (p->isCollidable() && p->isObstacle() && !object->isIgnored(p->entity().id))
      obstacles.push_back(p->entity().getGlobalBox());
  });
  std::for_each(mMovingComps.begin(), mMovingComps.end(), [&](MovingPhysicsComponent *p) {
    if (p->isCollidable() && p->isObstacle() && !object->isIgnored(p->entity().id))
      obstacles.push_back(p->entity().getGlobalBox());
  });

  if (direction.x > 0) {
    // Find the closest obstacle on X
    for (auto it=obstacles.begin(); it != obstacles.end(); it++) {
      if (box.x < it->x && box.y + box.h > it->y && box.y < it->y + it->h)
        destFacingPoint.x = std::min(destFacingPoint.x, it->x);
    }

    // Move on the X axis up to the closest obstacle found
    box.x = destFacingPoint.x - box.w;
  }
  else if (direction.x < 0) {
    for (auto it=obstacles.begin(); it != obstacles.end(); it++) {
      if (box.x > it->x && box.y + box.h > it->y && box.y < it->y + it->h)
        destFacingPoint.x = std::max(destFacingPoint.x, it->x + it->w);
    }

    box.x = destFacingPoint.x;
  }

  // Same for the Y axis
  if (direction.y > 0) {
    for (auto it=obstacles.begin(); it != obstacles.end(); it++) {
      if (box.y < it->y && box.x + box.w > it->x && box.x < it->x + it->w)
        destFacingPoint.y = std::min(destFacingPoint.y, it->y);
    }

    box.y = destFacingPoint.y - box.h;
  }
  else if (direction.y < 0) {
    for (auto it=obstacles.begin(); it != obstacles.end(); it++) {
      if (box.y > it->y && box.x + box.w > it->x && box.x < it->x + it->w)
        destFacingPoint.y = std::max(destFacingPoint.y, it->y + it->h);
    }

    box.y = destFacingPoint.y;
  }

  entity.setGlobalPos(Vector<float>(box.x, box.y));
  return box.x == dest.x && box.y == dest.y;
}

void PhysicsManager::checkCollisions(MovingPhysicsComponent *object)
{
  mLevel.checkTileCollisions(*object);
  const Rect<float> &box = object->entity().getGlobalBox();

  for (StaticPhysicsComponent *other : mStaticComps) {

    if (other->isCollidable() && box.touches(other->entity().getGlobalBox()) && !object->isIgnored(other->entity().id)) {
      object->collide(*other);
      other->collide(*object);
    }
  }

  // We only need to check moving components after the given component,
  // since any colllision with the preceding ones has already been handled at that point.

  auto it = std::find(mMovingComps.begin(), mMovingComps.end(), object);
  assert(it != mMovingComps.end());

  for (it += 1; it != mMovingComps.end(); it++) {

    if ((*it)->isCollidable() && box.touches((*it)->entity().getGlobalBox()) && !object->isIgnored((*it)->entity().id) && !(*it)->isIgnored(object->entity().id)) {
      object->collide(**it);
      (*it)->collide(*object);
    }
  }
}

std::vector<Rect<float> > PhysicsManager::getObstaclesInArea(const Rect<float> &area, const MovingPhysicsComponent &object)
{
  std::vector<Rect<float>> obstacles = mLevel.getObstaclesInArea(area, object);

  std::for_each(mStaticComps.begin(), mStaticComps.end(), [&](StaticPhysicsComponent *p) {
    if (p->isCollidable() && p->isObstacle() && area.intersects(p->entity().getGlobalBox()) && !object.isIgnored(p->entity().id))
      obstacles.push_back(p->entity().getGlobalBox());
  });
  std::for_each(mMovingComps.begin(), mMovingComps.end(), [&](MovingPhysicsComponent *p) {
    if (p->isCollidable() && p->isObstacle() && area.intersects(p->entity().getGlobalBox()) && !object.isIgnored(p->entity().id))
      obstacles.push_back(p->entity().getGlobalBox());
  });

  return obstacles;
}

void PhysicsManager::addComponent(MovingPhysicsComponent *cmp)
{
  assert(std::find(mMovingComps.begin(), mMovingComps.end(), cmp) == mMovingComps.end());

  mMovingComps.push_back(cmp);
}

void PhysicsManager::removeComponent(MovingPhysicsComponent *cmp)
{
  auto it = std::find(mMovingComps.begin(), mMovingComps.end(), cmp);
  assert(it != mMovingComps.end());

  mMovingComps.erase(it);
}

void PhysicsManager::addComponent(StaticPhysicsComponent *cmp)
{
  assert(std::find(mStaticComps.begin(), mStaticComps.end(), cmp) == mStaticComps.end());

  mStaticComps.push_back(cmp);
}

void PhysicsManager::removeComponent(StaticPhysicsComponent *cmp)
{
  auto it = std::find(mStaticComps.begin(), mStaticComps.end(), cmp);
  assert(it != mStaticComps.end());

  mStaticComps.erase(it);
}

} // namespace game
