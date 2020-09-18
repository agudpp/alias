#ifndef TOOLBOX_LOGGER_UTILS_H_
#define TOOLBOX_LOGGER_UTILS_H_

#include <string>
#include <memory>
#include <vector>

#include <toolbox/logger/Loggers.h>

namespace toolbox {


class RotativeLogger {
  public:
    RotativeLogger(const std::string& log_name, const std::string& log_folder);
    ~RotativeLogger(void);

  private:

    typedef std::shared_ptr<Logger> LoggerPtr;

  private:
    std::vector<LoggerPtr> loggers_;
};

class ConsoleLoggerWrapper {
  public:
    ConsoleLoggerWrapper(void);
    ~ConsoleLoggerWrapper(void);

  private:

    typedef std::shared_ptr<Logger> LoggerPtr;

  private:
    LoggerPtr logger_;
};


#define CREATE_CONSOLE_LOGGER() toolbox::ConsoleLoggerWrapper console_logger
#define CREATE_FILE_MODULE_LOGGER(module_name, log_folder) toolbox::RotativeLogger file_module_logger(module_name, log_folder)


} // namespace toolbox

#endif // TOOLBOX_LOGGER_UTILS_H_
