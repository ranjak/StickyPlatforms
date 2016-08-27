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
  /**
   * @brief The Priority enum defines the possible priorities for a log message.
   */
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

  /**
   * @brief log Print a log message with the given priority.
   */
  void log(Priority, const std::string&);

  /**
   * @brief setLevel Set the maximum log level for the given logger.
   * Log messages below this level will be ignored.
   * @param maxLevel Maximum priority level for this logger.
   */
  void setLevel(Priority maxLevel);

  Log();
  Log(std::ostream &output);

  // It doesn't make sense to copy loggers
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
