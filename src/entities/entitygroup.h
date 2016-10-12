#ifndef ENTITYGROUP_H
#define ENTITYGROUP_H

#include <cstdint>

namespace game {

enum EntityGroup : std::uint8_t
{
  NONE = 0,
  ALLY = 1,
  ENEMY = 1 << 1,
  ANY = 0xFF
};

} // namespace game

#endif // ENTITYGROUP_H
