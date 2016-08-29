#ifndef VECTOR_H
#define VECTOR_H

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

#endif // VECTOR_H
