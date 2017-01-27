#include "text.h"
#include "log.h"
#include "util_sdl.h"
#include "display.h"
#include "resource.h"
#include "SDL_ttf.h"
#include "SDL.h"
#include <memory>
#include <cmath>

namespace game {

namespace {

Text::FontUptr openFont(const std::string &fontFile, int fontSize)
{
  if (!TTF_WasInit() && TTF_Init() < 0)
    game::error(std::string("Couldn't initialize SDL_ttf: ")+TTF_GetError());

  Text::FontUptr font(TTF_OpenFont(Resource::getInstance().assetPath(fontFile).c_str(), fontSize), TTF_CloseFont);

  if (!font)
    game::error(std::string("Couldn't open font ")+fontFile+" : "+TTF_GetError());

  return font;
}

} // namespace

const std::string Text::defaultFont("assets/fonts/FreeMono.ttf");

Text::Text(Display &renderer, const std::string &text, int fontSize, const std::string &fontFile, const Color &color) :
  Texture(renderer),
  mSize(fontSize),
  mRenderScale(renderer.getScale().x),
  mFont(openFont(fontFile, static_cast<int>(std::round(mSize*mRenderScale)))),
  mFontFile(fontFile),
  mText(text),
  mColor(color)
{
  render();
}

void Text::setSize(int size)
{
  mFont = openFont(mFontFile, static_cast<int>(std::round(size*mRenderScale)));
  mSize = size;
  render();
}

Vector<int> Text::getSize() const
{
  return Texture::getSize() * (1.f / mRenderScale);
}

void Text::setText(const std::string &text, const Color &color)
{
  if (mText == text && mColor == color)
    return;

  mText = text;
  mColor = color;
  render();
}

void Text::draw(Display &target, int x, int y)
{
  // I always have the same scale on both axis
  float currentScale = target.getScale().x;

  if (std::round(currentScale*mSize) != std::round(mRenderScale*mSize)) {
    mRenderScale = currentScale;
    setSize(mSize);
  }

  // Draw the text using actual, non-scaled coordinates
  target.setScale(1.f);
  Texture::draw(target, static_cast<int>(x*mRenderScale), static_cast<int>(y*mRenderScale));
  target.setScale(mRenderScale);
}

void Text::draw(Display &target, const Vector<float> &pos)
{
  draw(target, static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void Text::render()
{
  SDL_Surface *newText = TTF_RenderUTF8_Blended(mFont.get(), mText.c_str(), getSdlColor(mColor));

  if (newText) {
    reset(newText);
    SDL_FreeSurface(newText);
  }
  else
    Log::getGlobal().get(Log::WARNING) << "Text: failed to render text: \"" << mText << "\"" << std::endl;
}


} // namespace game
