#include "physicsobject.h"

namespace game {

const int PhysicsObject::GRAVITY_ACCEL = 10;
int PhysicsObject::metersToPxRatio = 80;

PhysicsObject::PhysicsObject(int mass) :
  forces(),
  mass(mass),
  gravityEnabled(true),
  gravity(0, mass*GRAVITY_ACCEL),
  speed(0, 0)
{

}

void PhysicsObject::update(uint32_t timestep)
{
  // Objects with infinite mass never move
  if (mass == 0)
    return;

  // Sum all forces
  Vector<int> accel(0, 0);
  for (Vector<int>& force : forces) {
    accel += force;
  }
  if (gravityEnabled) {
    accel += gravity;
  }

  // Compute acceleration
  accel.x /= mass;
  accel.y /= mass;

  // Compute speed
  speed.x += (float)accel.x * timestep / 1000.f;
  speed.y += (float)accel.y * timestep / 1000.f;

  forces.clear();
}

void PhysicsObject::applyForce(Vector<int> &force)
{
  forces.push_back(force);
}

void PhysicsObject::setGravity(bool enabled)
{
  gravityEnabled = enabled;
}

const Vector<float> &PhysicsObject::getSpeed()
{
  return speed;
}

}
