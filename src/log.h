#ifndef LOG_H
#define LOG_H

#include <iostream>

// Helper define for logging, disabled in release mode
#ifndef NDEBUG
# define glog(severity, msg); game::Log::getGlobal().log(severity, msg);
#else
# define glog(severity, msg);
#endif

namespace game {

/**
 * @brief Log objects can log to a given std::ostream.
 * By default, they will log to stdout.
 */
class Log
{
public:
  enum class Priority {
    ERROR,
    WARNING,
    INFO,
    DBG,
    TRACE
  };

  /**
   * @brief getGlobal Retrieve the static global logger object.
   * @return A reference to the statically created global logger object.
   */
  static Log& getGlobal();

  void log(Priority, const std::string&);
  void setLevel(Priority maxLevel);

  Log();
  Log(std::ostream &output);
  // Our log singlenton can't be copied
  Log(Log const&) = delete;
  void operator=(Log const&) = delete;

private:
  std::ostream& output;
  Priority level;
};

/**
 * @brief Print an error message on the global log.
 */
void error(std::string msg);

}

#endif // LOG_H
