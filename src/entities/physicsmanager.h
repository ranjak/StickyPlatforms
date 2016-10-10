#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <vector>

namespace game {

class PhysicsComponent;
class Level;
template<typename T> class Vector;

class PhysicsManager
{
public:
  PhysicsManager(Level &level);

  bool moveObject(PhysicsComponent *object, const Vector<float> &dest);

  void checkCollisions(PhysicsComponent *object);

  void addComponent(PhysicsComponent *cmp);
  void removeComponent(PhysicsComponent *cmp);

private:
  std::vector<PhysicsComponent *> mComponents;
  Level &mLevel;
};

} // namespace game

#endif // PHYSICSMANAGER_H
