#ifndef DISPLAY_H
#define DISPLAY_H

#include "gamevector.h"
#include "rect.h"
#include <string>

struct SDL_Window;
struct SDL_Renderer;

namespace game {

class Game;
class Color;

class Display
{
public:
  /**
   * @brief Initialize the display components (window, renderer, etc.)
   */
  Display(int winW, int winH, const std::string &windowTitle="StickyPlatforms");
  ~Display();


  void setLogicalSize(int w, int h);

  void setScale(float scaleFactor);

  void toggleFullscreen();

  Rect<int> getViewport();
  Vector<int> getOutputSize();
  Vector<float> getScale();

  /**
   * @brief render one frame.
   */
  void render(Game &game);

  void clear(const Color& color);

  SDL_Renderer* getRenderer();

  Vector<int> getWindowSize();

private:
  void init(int winW, int winH);

private:
  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
  // Logical size, different from the window size.
  Vector<int> mCameraSize;
  std::string mWindowTitle;
  bool mFullscreen;
};

}

#endif // DISPLAY_H
