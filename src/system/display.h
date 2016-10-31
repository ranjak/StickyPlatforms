#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"
#include "gamevector.h"
#include "rect.h"

namespace game {

class GameState;

class Display
{
public:
  /**
   * @brief Initialize the display components (window, renderer, etc.)
   */
  Display(int winW, int winH);
  ~Display();


  void setLogicalSize(int w, int h);

  void setScale(float scaleFactor);

  Rect<int> getViewport();
  Vector<int> getOutputSize();
  Vector<float> getScale();

  /**
   * @brief render one frame.
   */
  void render(GameState &game);

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
