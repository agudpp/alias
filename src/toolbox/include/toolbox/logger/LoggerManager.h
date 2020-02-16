/*
 * File LoggerManager.h, LoggerManager
 * Created on 02/12/2014.
 * Author: Agustin
 *
 * Happen
*/

#ifndef TOOLBOX_LOGGERMANAGER_H_
#define TOOLBOX_LOGGERMANAGER_H_

#include <vector>
#include <string>
#include <algorithm>


namespace toolbox {

////////////////////////////////////////////////////////////////////////////////
///                         HELPER LOGGING MACROS
////////////////////////////////////////////////////////////////////////////////
/// \note There is also another way to log information with macros already for
///       debugging in the DebugUtils.h file that use the logger already. But
///       this macros will be disabled in release mode, so if you want to
///       always LOG information you can use this methods.
/// TODO: maybe we can change the debugXXX methods by LOG_XXX methods so we don't
///       end with multiple macros



////////////////////////////////////////////////////////////////////////////////

// define the type of logging
enum LogType {
    LT_ERROR = 0,
    LT_WARNING,
    LT_TODO,
    LT_OPTIMIZATION,
    LT_INFO,
    LT_STATUS,
};

// define the type of level
enum LogLevel {
    LL_0 = 0,   // Verbose (0 + 1 + 2)
    LL_1,       // standard (1 + 2)
    LL_2,       // low (2 only)
};

///
/// \brief The Logger class will be in charge to do the real logging (in a file
///        or any other place..)
///
class Logger {
public:
    Logger(void) = default;
    virtual ~Logger(void) = default;

    ///
    /// \brief Interface that the Logger's should implement
    /// \param msg  The message
    /// \param lt   The log type
    ///
    virtual void
    logMessage(const char* msg, LogType lt) = 0;
};

class LoggerManager
{
public:

    static LoggerManager&
    instance(void)
    {
        static LoggerManager instance;
        return instance;
    }

    ///
    /// \brief configureLevel is used to set which kind of level we want to
    ///        log, we will log all the messages with level >= logLevel
    /// \param logLevel The level to log (lower bound)
    ///
    inline void
    configureLevel(LogLevel logLevel);

    ///
    /// \brief addLogger to handle the logs
    /// \param logger   the instance
    ///
    inline void
    addLogger(Logger* logger);

    ///
    /// \brief removeLogger removes a logger
    /// \param logger the logger to remove
    ///
    inline void
    removeLogger(Logger* logger);


    inline void
    logMessage(const std::string& msg, LogType lt, LogLevel ll = LogLevel::LL_0);
    inline void
    logMessage(const char* msg, LogType lt, LogLevel ll = LogLevel::LL_0);

private:
    LoggerManager() : m_level(LogLevel::LL_0) {}

private:
    std::vector<Logger*> m_loggers;
    LogLevel m_level;
};






////////////////////////////////////////////////////////////////////////////////
// inline stuff
//

inline void
LoggerManager::configureLevel(LogLevel logLevel)
{
    m_level = logLevel;
}

inline void
LoggerManager::addLogger(Logger* logger)
{
    m_loggers.push_back(logger);
}

inline void
LoggerManager::removeLogger(Logger* logger)
{
  auto it = std::find(m_loggers.begin(), m_loggers.end(), logger);
  if (it != m_loggers.end()) {
    m_loggers.erase(it);
  }
}


inline void
LoggerManager::logMessage(const std::string& msg, LogType lt, LogLevel ll)
{
    logMessage(msg.c_str(), lt, ll);
}

inline void
LoggerManager::logMessage(const char* msg, LogType lt, LogLevel ll)
{
    if (ll < m_level) return;
    for (Logger* logger : m_loggers) logger->logMessage(msg, lt);
}



} // namespace toolbox

#endif // TOOLBOX_LOGGERMANAGER_H_
