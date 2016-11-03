#ifndef TRIGGERMSG_H
#define TRIGGERMSG_H

#include "message.h"
#include "entity.h"
#include <memory>

namespace game {

struct TriggerMsg : public Message
{
  enum class Type { Enter, Exit };

  TriggerMsg(Type type, EntityID trigger, std::unique_ptr<Message> embeddedMsg=nullptr) : Message(toMsgType(type)), triggerEntity(trigger), msg(std::move(embeddedMsg)) {}

  EntityID triggerEntity;
  std::unique_ptr<Message> msg;

private:
  Message::Type toMsgType(Type t) { return (t == Type::Enter) ? TriggerEnter : TriggerExit; }
};

}

#endif // TRIGGERMSG_H
