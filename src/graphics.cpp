#include "graphics.h"
#include "gamevector.h"

namespace game {


Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

void Graphics::draw(Display &target, const Vector<int> &pos) const
{
  draw(target, pos.x, pos.y);
}

} // namespace game
