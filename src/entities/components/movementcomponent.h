#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "component.h"
#include "gamevector.h"
#include <cstdint>

namespace game {

class Entity;
class GameState;

class MovementComponent : public Component
{
public:
  MovementComponent(Entity &entity);

  void update(std::uint32_t step, GameState &game);

  Vector<float> &velocity();

private:
  Vector<float> mVelocity;
  Vector<float> mRemainder;
  Entity& mEntity;
};

} // namespace game

#endif // MOVEMENT_H