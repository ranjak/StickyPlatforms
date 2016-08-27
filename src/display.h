#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "gamestate.h"

namespace game {


class Display
{
public:
  Display();
  ~Display();

  /**
   * @brief Initialize the display components (window, renderer, etc.)
   */
  void init();

  /**
   * @brief render one frame.
   */
  void render(const GameState& game);

private:
  SDL_Window* window;
  SDL_Renderer* renderer;
};

}

#endif // RENDERER_H
