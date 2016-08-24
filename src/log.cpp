#include "log.h"

namespace game {


Log::Log() :
  Log(std::cout)
{
}


Log::Log(std::ostream& output) :
  output(output),
  level(Priority::DBG)
{
}


Log& Log::getGlobal()
{
  static Log globalLogger;

  return globalLogger;
}


void Log::log(Priority sev, const std::string& msg)
{
  if (sev >= level)
  {
    std::string priorityStr;
    switch (sev) {
    case Priority::ERROR:
      priorityStr = "[ERROR] ";
      break;
    case Priority::WARNING:
      priorityStr = "[WARNING] ";
      break;
    case Priority::INFO:
      priorityStr = "[INFO] ";
      break;
    case Priority::DBG:
      priorityStr = "[DEBUG] ";
      break;
    case Priority::TRACE:
      priorityStr = "[TRACE] ";
      break;
    default:
      priorityStr = "";
      break;
    }

    output << priorityStr << msg << std::endl;
  }
}


void Log::setLevel(Priority maxLevel)
{
  this->level = maxLevel;
}


void error(std::string msg)
{
  Log::getGlobal().log(Log::Priority::ERROR, msg);
}

}
