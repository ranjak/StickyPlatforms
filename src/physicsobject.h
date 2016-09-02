#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <vector>
#include <cstdint>
#include "gamevector.h"

namespace game {


/**
 * @brief The PhysicsObject class handles physics logic for an object.
 * A Physics Object has mass (in kg), speed (in m/s) and acceleration.
 * It is possible to apply forces to it, which in turn determine its acceleration and speed.
 * An object with mass 0 is considered to have infinite mass.
 */
class PhysicsObject
{
public:
  // Gravity strength in m/s²
  static const int GRAVITY_ACCEL;
  // Ratio to convert distances from meters (used by this class) to pixels.
  static int metersToPxRatio;

  PhysicsObject(int mass = 0);

  void update(std::uint32_t timestep);
  void applyForce(Vector<int>& force);
  void setGravity(bool enabled);
  const Vector<float> &getVelocity();

private:
  std::vector<Vector<int>> forces;
  int mass;
  bool gravityEnabled;
  Vector<int> gravity;
  Vector<float> velocity;
};

}

#endif // PHYSICSOBJECT_H
