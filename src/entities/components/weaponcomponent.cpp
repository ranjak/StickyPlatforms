#include "weaponcomponent.h"
#include "swordcomponent.h"
#include "entity.h"
#include "entitymanager.h"
#include "childremovedmsg.h"
#include <cassert>

namespace game {

WeaponComponent::WeaponComponent(Entity &owner) :
  mSword(owner.manager().makeEntity("sword", owner.getName()+"_sword", Rect<float>(0.f, 0.f, 32.f, 8.f), owner.id)),
  mSwordComp(owner.manager().getEntity(mSword)->getComponent<SwordComponent>())
{

}

WeaponComponent::~WeaponComponent()
{
  if (mSword != Entity::none)
    mSwordComp->getEntity().kill();
}

void WeaponComponent::receiveMessageDelegate(Message &msg)
{
  if (msg.type == Message::ChildRemoved && static_cast<ChildRemovedMsg &>(msg).id == mSword) {
    mSword = Entity::none;
    mSwordComp = nullptr;
    // TODO the component should probably destroy itself at that point
  }
}

void WeaponComponent::swing(int direction)
{
  if (mSwordComp)
    mSwordComp->swing(direction);
}

} // namespace game
