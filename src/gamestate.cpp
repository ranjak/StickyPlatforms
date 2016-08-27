#include "gamestate.h"


namespace game {

const std::uint32_t TIMESTEP = 10;

GameState::GameState() :
  bindings(),
  entities()
{

}


void GameState::update(uint32_t step, const InputHandler& input)
{
  // Find out which commands are active

}


} //namespace game
