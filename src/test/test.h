#ifndef TEST_H
#define TEST_H

#include "world/level.h"
#include <memory>

class Test
{
public:
  static std::unique_ptr<game::Level> makeLevel();
};

#endif // TEST_H
