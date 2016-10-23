#include "gamevector.h"

namespace game {


template<typename T>
Vector<T> Vector<T>::operator/(float scalar)
{
  return Vector<T>(x / scalar, y / scalar);
}

template<typename T>
Vector<T> Vector<T>::operator*(float scalar)
{
  return Vector<T>(x * scalar, y * scalar);
}

template<typename T>
Vector<T> &Vector<T>::operator +=(const Vector<T> &right)
{
  x += right.x;
  y += right.y;

  return *this;
}



// Template definitions for needed Vector specializations
template class Vector<int>;
template class Vector<float>;

} // namespace game
