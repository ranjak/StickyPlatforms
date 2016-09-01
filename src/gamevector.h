#ifndef GAMEVECTOR_H
#define GAMEVECTOR_H

namespace game {


template<typename T>
class Vector
{
public:
  Vector();
  Vector(T x, T y);


  T x;
  T y;
};

} //namespace game

#endif // GAMEVECTOR_H
