#include "gamevector.h"

namespace game {

template<typename T>
Vector<T>::Vector() :
  x(),
  y()
{

}


template<typename T>
Vector<T>::Vector(T x, T y) :
  x(x),
  y(y)
{}

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



// Template definitions for needed Vector specializations
template class Vector<int>;
template class Vector<float>;

} // namespace game
