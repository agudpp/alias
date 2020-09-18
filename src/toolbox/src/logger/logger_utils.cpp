#include <toolbox/logger/logger_utils.h>

#include <ctime>
#include <iostream>
#include <sstream>

#include <toolbox/logger/LoggerManager.h>


namespace toolbox {

static std::string
getLogFilePath(const std::string& log_name, const std::string& log_folder)
{
  // TODO(agudpp) #67:  here we may want to use a better approach for setting the file name
  //                    and do a rotation logs? or have 2 files with specific file size
  //                    (1 meg each, if it is bigger go to the next, between 2 or 3 files)
  // for now we will create a new one every time
  const std::time_t timestamp = std::time(nullptr);
  std::stringstream result;
  result << log_folder << log_name << "_" << timestamp << ".log";
  return result.str();
}

RotativeLogger::RotativeLogger(const std::string& log_name, const std::string& log_folder)
{
  const std::string file_path = getLogFilePath(log_name, log_folder);
  loggers_.push_back(LoggerPtr(new toolbox::FileLogger(file_path)));
  for (LoggerPtr& logger : loggers_) {
      toolbox::LoggerManager::instance().addLogger(logger.get());
  }
  toolbox::LoggerManager::instance().configureLevel(toolbox::LogLevel::LL_0);
}

RotativeLogger::~RotativeLogger(void)
{
  for (const LoggerPtr& logger : loggers_) {
    toolbox::LoggerManager::instance().removeLogger(logger.get());
  }
}

ConsoleLoggerWrapper::ConsoleLoggerWrapper(void)
{
  logger_.reset(new toolbox::ConsoleLogger);
  toolbox::LoggerManager::instance().addLogger(logger_.get());
}

ConsoleLoggerWrapper::~ConsoleLoggerWrapper(void)
{
  toolbox::LoggerManager::instance().removeLogger(logger_.get());
}


} // namepsace toolbox
