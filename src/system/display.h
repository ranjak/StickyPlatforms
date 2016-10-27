#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"
#include "gamevector.h"

namespace game {

class GameState;
template<typename T> class Vector;

class Display
{
public:
  /**
   * @brief Initialize the display components (window, renderer, etc.)
   */
  Display(int winW, int winH);
  ~Display();


  void setLogicalSize(int w, int h);

  void useWindowCoordinates();

  /**
   * @brief render one frame.
   */
  void render(const GameState& game);

  SDL_Renderer* getRenderer();

  Vector<int> getWindowSize();

private:
  void init(int winW, int winH);

private:
  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
  // Logical size, different from the window size.
  Vector<int> mCameraSize;
};

}

#endif // DISPLAY_H
