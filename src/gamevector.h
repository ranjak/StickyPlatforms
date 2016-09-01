#ifndef GAMEVECTOR_H
#define GAMEVECTOR_H

namespace game {


template<typename T>
class Vector
{
public:
  Vector();
  Vector(T x, T y);

  void operator+=(const Vector& rhs);

  T x;
  T y;
};


} //namespace game

#endif // GAMEVECTOR_H
