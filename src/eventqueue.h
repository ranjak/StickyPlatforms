#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <mutex>
#include <queue>
#include <functional>
#include <memory>

namespace game
{

class EventQueue
{
public:
  EventQueue();

  void push(std::function<void()> action);

  void processEvents();

private:
  bool processSingleEvent();

private:
  std::mutex mMutex;
  std::queue<std::function<void()>> mEvents;
};

} // namespace game

#endif // EVENTQUEUE_H