#ifndef RESOURCE_H
#define RESOURCE_H

#include <memory>
#include <string>

namespace game {


class Resource
{
public:
  static Resource &getInstance();

  std::string assetPath(const std::string &assetPath);

private:
  Resource();

private:
  static std::unique_ptr<Resource> instance;

  std::string resourcePath;
  std::string userPath;
};

}

#endif // RESOURCE_H
