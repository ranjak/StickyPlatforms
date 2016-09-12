#ifndef PLATFORM_H
#define PLATFORM_H

#include "entity.h"
#include "rectangle.h"

namespace game {

class Camera;

class Platform : public Entity
{
public:
  Platform();
  Platform(int x, int y, int w=50, int h=10);

  void update(std::uint32_t step, GameState& game) override;

  void draw(Display& target, const Camera &cam) const override;

private:
  Rectangle mGraphics;
};

} // namespace game

#endif // PLATFORM_H
