#include "log.h"

namespace game {


Log::Log() :
  Log(std::cout)
{
}


Log::Log(std::ostream& output) :
  mOutput(output),
  mLevel(Priority::DBG)
{
}


Log& Log::getGlobal()
{
  static Log globalLogger;

  return globalLogger;
}


void Log::log(Priority sev, const std::string& msg)
{
  if (sev >= mLevel)
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

    mOutput << priorityStr << msg << std::endl;
  }
}


void Log::setLevel(Priority maxLevel)
{
  this->mLevel = maxLevel;
}


void error(std::string msg)
{
  Log::getGlobal().log(Log::Priority::ERROR, msg);
}

}
