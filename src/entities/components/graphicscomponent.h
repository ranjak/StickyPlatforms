#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

#include "component.h"
#include "graphics.h"
#include "entity.h"
#include <memory>

namespace game {

class Display;

class GraphicsComponent : public Component
{
public:
  GraphicsComponent(std::unique_ptr<Graphics> graphics);

  void draw(Display &display, const Vector<float> &pos) const;

private:
  std::unique_ptr<Graphics> mGraphics;
};

} // namespace game

#endif // GRAPHICSCOMPONENT_H
