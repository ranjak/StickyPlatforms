#ifndef TEST_H
#define TEST_H

#include <memory>

namespace game {
class Display;
class Level;
}

class Test
{
public:
  static std::unique_ptr<game::Level> makeLevel(game::Display &display);
};

#endif // TEST_H
