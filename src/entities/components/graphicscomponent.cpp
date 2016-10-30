#include "graphicscomponent.h"
#include "gamestate.h"

namespace game {

GraphicsComponent::GraphicsComponent(std::unique_ptr<Graphics> graphics) :
  mGraphics(std::move(graphics))
{

}

void GraphicsComponent::draw(Display &display, const Vector<float> &pos) const
{
  mGraphics->draw(display, GameState::current().getCamera().toCamCoords(pos));
}

} // namespace game
