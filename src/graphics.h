#ifndef GRAPHICS_H
#define GRAPHICS_H


namespace game {

class Display;
template<typename T> class Rect;

class Graphics
{
public:
  // TODO bind all graphics objects to a single renderer at creation time (remove draw's target parameter)
  Graphics();
  virtual ~Graphics();

  virtual void draw(Display &target, int x, int y) const = 0;

  virtual void draw(Display &target, const Rect<int> &dest) const = 0;

};

} //namespace game

#endif // GRAPHICS_H
