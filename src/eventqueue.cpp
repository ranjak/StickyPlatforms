#include "eventqueue.h"

namespace game
{

EventQueue::EventQueue() :
  mMutex(),
  mEvents()
{
}

void EventQueue::push(std::function<void()> action)
{
  std::lock_guard<std::mutex> l(mMutex);

  mEvents.push(action);
}

void EventQueue::processEvents()
{
  while (processSingleEvent());
}

bool EventQueue::processSingleEvent()
{
  std::function<void()> action;

  {
    std::lock_guard<std::mutex> l(mMutex);
    
    if (mEvents.size() > 0) {
      action = mEvents.front();
      mEvents.pop();
    }
    else
      return false;
  }

  action();
  return true;
}

} // namespace game