#ifndef BASICAICOMPONENT_H
#define BASICAICOMPONENT_H

#include "component.h"

namespace game {


class BasicAiComponent : public Component
{
public:
  BasicAiComponent(Entity &owner);

  void receiveMessage(Message &message) override;

private:
  Entity &mOwner;
};

} // namespace game

#endif // BASICAICOMPONENT_H
