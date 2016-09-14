#include "resource.h"
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
  return execPath + assetPath;
}

Resource::Resource() :
  execPath(),
  userPath()
{
  char *exec = SDL_GetBasePath();
  if (exec == nullptr)
    throw std::runtime_error(SDL_GetError());

  char *user = SDL_GetBasePath();
  if (user == nullptr)
    throw std::runtime_error(SDL_GetError());

  execPath = exec;
  userPath = user;

  SDL_free(exec);
  SDL_free(user);
}

} // namespace game
