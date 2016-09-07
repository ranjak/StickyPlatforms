#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"
#include "gamestate.h"

namespace game {

class GameState;

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

  SDL_Renderer* getRenderer();

private:
  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
};

}

#endif // DISPLAY_H
