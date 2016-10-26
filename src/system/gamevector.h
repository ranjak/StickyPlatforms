#ifndef GAMEVECTOR_H
#define GAMEVECTOR_H

#include <iostream>

namespace game {


template<typename T>
class Vector
{
public:
  constexpr Vector() : x(), y() {}
  constexpr Vector(T x, T y) : x(x), y(y) {}

  template<typename U>
  Vector(const Vector<U> &v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

  Vector<T> operator /(float scalar);
  Vector<T> operator *(float scalar);

  Vector<T> &operator +=(const Vector<T> &right);

  template<typename U>
  explicit operator Vector<U>() const
  {
    return Vector<U>(*this);
  }

  friend std::ostream &operator<<(std::ostream &os, const Vector &v)
  {
    return os << "("<<v.x<<","<<v.y<<")";
  }

public:
  T x;
  T y;
};

template<typename T>
inline Vector<T> operator +(Vector<T> left, const Vector<T> &right)
{
  left += right;
  return left;
}

template<typename T>
inline bool operator ==(const Vector<T> &left, const Vector<T> &right)
{
  return left.x == right.x && left.y == right.y;
}

template<typename T>
inline bool operator !=(const Vector<T> &left, const Vector<T> &right)
{
  return !(left == right);
}

} //namespace game

#endif // GAMEVECTOR_H
