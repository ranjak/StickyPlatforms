#ifndef GRAPHICS_H
#define GRAPHICS_H


namespace game {

class Display;
template<typename T> class Rect;
template<typename T> class Vector;

class Graphics
{
public:
  // TODO bind all graphics objects to a single renderer at creation time (remove draw's target parameter)
  Graphics();
  virtual ~Graphics();

  virtual void draw(Display &target, int x, int y) const = 0;
  void draw(Display &target, const Vector<int> &pos) const;

  virtual void draw(Display &target, const Rect<int> &dest) const = 0;

};

} //namespace game

#endif // GRAPHICS_H
