#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"

namespace game {

class GameState;
template<typename T> class Vector;

class Display
{
public:
  Display();
  ~Display();

  /**
   * @brief Initialize the display components (window, renderer, etc.)
   */
  void init(int winW, int winH);

  void setCameraSize(int w, int h);

  /**
   * @brief render one frame.
   */
  void render(const GameState& game);

  SDL_Renderer* getRenderer();

  Vector<int> getWindowSize();

private:
  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
};

}

#endif // DISPLAY_H
