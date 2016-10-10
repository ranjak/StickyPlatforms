#include "physicsmanager.h"
#include "gamevector.h"
#include "rect.h"
#include "entity.h"
#include "physicscomponent.h"
#include "world/level.h"
#include <cassert>
#include <algorithm>

namespace game {

PhysicsManager::PhysicsManager(Level &level) :
  mComponents(),
  mLevel(level)
{

}

bool PhysicsManager::moveObject(PhysicsComponent *object, const Vector<float> &dest)
{
  Entity &entity = object->entity();

  Rect<float> box = entity.getGlobalBox();
  Vector<float> direction(dest.x - box.x, dest.y - box.y);
  Vector<float> facingPoint;

  facingPoint.x = (direction.x >= 0) ? box.x + box.w : box.x;
  facingPoint.y = (direction.y >= 0) ? box.y + box.h : box.y;

  Vector<float> destFacingPoint(facingPoint.x + direction.x, facingPoint.y + direction.y);

  // Do not go beyond the level's boundaries
  const Vector<float> &levelSize = mLevel.getPixelSize();

  if (destFacingPoint.x < 0)
    destFacingPoint.x = 0;
  else if (destFacingPoint.x > levelSize.x)
    destFacingPoint.x = levelSize.x;

  if (destFacingPoint.y < 0)
    destFacingPoint.y = 0;
  else if (destFacingPoint.y > levelSize.y)
    destFacingPoint.y = levelSize.y;

  if (!object->isObstacle()) {

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

  std::vector<Rect<float>> obstacles = mLevel.getObstaclesInArea(movementArea);

  std::for_each(mComponents.begin(), mComponents.end(), [&](PhysicsComponent *p) { if (p->isObstacle()) obstacles.push_back(p->entity().getGlobalBox()); });

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

void PhysicsManager::checkCollisions(PhysicsComponent *object)
{
  mLevel.checkTileCollisions(*object);
  const Rect<float> &box = object->entity().getGlobalBox();

  // We only need to check components after the given component,
  // since any colllision with the preceding ones has laready been handled at that point.

  auto it = std::find(mComponents.begin(), mComponents.end(), object);
  assert(it != mComponents.end());

  for (it += 1; it != mComponents.end(); it++) {

    if (box.touches((*it)->entity().getGlobalBox())) {
      object->collide(**it);
      (*it)->collide(*object);
    }
  }
}

void PhysicsManager::addComponent(PhysicsComponent *cmp)
{
  assert(std::find(mComponents.begin(), mComponents.end(), cmp) == mComponents.end());

  mComponents.push_back(cmp);
}

void PhysicsManager::removeComponent(PhysicsComponent *cmp)
{
  auto it = std::find(mComponents.begin(), mComponents.end(), cmp);
  assert(it != mComponents.end());

  mComponents.erase(it);
}

} // namespace game
