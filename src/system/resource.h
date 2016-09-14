#ifndef RESOURCE_H
#define RESOURCE_H

#include <memory>
#include <iostream>

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

  std::string execPath;
  std::string userPath;
};

}

#endif // RESOURCE_H
