#ifndef PLAYERCUBE_H
#define PLAYERCUBE_H

#include "graphics.h"
#include <cstdint>

namespace game {


class PlayerCube : public Graphics
{
public:
  PlayerCube();

  PlayerCube(int x, int y, int w, int h);

  void draw(Display& target) const override;

  void setPos(int x, int y);
  void setSize(int w, int h);

private:
  // Fill color of the cube
  std::uint8_t r, g, b, a;
  // Size and position in window coordinates
  SDL_Rect sizePos;
};

} // namespace game

#endif // PLAYERCUBE_H
