#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H

#include "component.h"
#include "entity.h"

namespace game {

class SwordComponent;
class Entity;

/**
 * @brief A simple "container" for a weapon (currently, only swords).
 * Acts as a proxy between the weapon and the entity that wields it.
 * Manages the weapon's life cycle.
 */
class WeaponComponent : public Component
{
public:
  WeaponComponent(Entity &owner);

  ~WeaponComponent();

  void receiveMessage(Message &msg) override;

  void swing(int direction);

private:
  static SwordComponent &makeSword(Entity &owner);

private:
  EntityID mSword;
  // Shortcut to mSword's control component
  SwordComponent *mSwordComp;
};

} // namespace game

#endif // WEAPONCOMPONENT_H
