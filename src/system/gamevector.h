#ifndef GAMEVECTOR_H
#define GAMEVECTOR_H

namespace game {


template<typename T>
class Vector
{
public:
  Vector();
  Vector(T x, T y);

  Vector<T> operator /(float scalar);
  Vector<T> operator *(float scalar);

  Vector<T> &operator +=(const Vector<T> &right);

  friend Vector<T> operator +(Vector<T> left, const Vector<T> &right)
  {
    left += right;
    return left;
  }

public:
  T x;
  T y;
};



} //namespace game

#endif // GAMEVECTOR_H
