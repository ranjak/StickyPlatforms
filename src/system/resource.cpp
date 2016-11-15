#include "resource.h"
#include "config.h"
#include "SDL.h"
#include <stdexcept>

namespace game {


std::unique_ptr<Resource> Resource::instance;

Resource &Resource::getInstance()
{
  if (instance.get() == nullptr)
    instance.reset(new Resource());

  return *instance;
}

std::string Resource::assetPath(const std::string &assetPath)
{
  // Absolute paths should already point to a correct, runtime-resolved location
  if (assetPath[0] == '/' || assetPath[1] == ':')
    return assetPath;

  return resourcePath + assetPath;
}

Resource::Resource() :
  resourcePath(),
  userPath()
{
  char *exec = SDL_GetBasePath();
  if (exec == nullptr)
    throw std::runtime_error(SDL_GetError());

  char *user = SDL_GetPrefPath("ranjak", "StickyPlatforms");
  if (user == nullptr)
    throw std::runtime_error(SDL_GetError());

  resourcePath = std::string(exec) + SPF_RESOURCE_PATH;
  userPath = user;

  SDL_free(exec);
  SDL_free(user);
}

} // namespace game
