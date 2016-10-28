#include "text.h"
#include "log.h"
#include "SDL_ttf.h"
#include "SDL.h"
#include "util_sdl.h"
#include <memory>

namespace game {

namespace {

std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)> makeText(const std::string &fontFile, int fontSize, const std::string &text, const Color &color)
{
  if(!TTF_WasInit() && TTF_Init()==-1)
    game::error(std::string("Couldn't initialize SDL_ttf: ")+TTF_GetError());

  std::unique_ptr<TTF_Font, void (*)(TTF_Font *)> font(TTF_OpenFont(fontFile.c_str(), fontSize), TTF_CloseFont);

  if (!font)
    game::error(std::string("Couldn't open font ")+fontFile+" : "+TTF_GetError());

  std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)> surface(TTF_RenderUTF8_Blended(font.get(), text.c_str(), getSdlColor(color)), SDL_FreeSurface);

  if (!surface)
    game::error(std::string("Couldn't render text: ")+TTF_GetError());

  return surface;
}

} // namespace

const std::string Text::defaultFont("assets/fonts/FreeMono.ttf");

Text::Text(Display &renderer, const std::string &text, int fontSize, const std::string &fontFile, const Color &color) :
  Texture(renderer, makeText(fontFile, fontSize, text, color).get())
{
}


} // namespace game
