#include "gamevector.h"

namespace game {


template<typename T>
Vector<T> Vector<T>::operator/(float scalar) const
{
  return Vector<T>(static_cast<T>(x / scalar), static_cast<T>(y / scalar));
}

template<typename T>
Vector<T> Vector<T>::operator*(float scalar) const
{
  return Vector<T>(static_cast<T>(x * scalar), static_cast<T>(y * scalar));
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
