/*
*  Useful functions used in debug
*/

#ifndef DEBUGUTIL_H_
#define DEBUGUTIL_H_

#include <functional>
#include <vector>
#include <cassert>
#include <memory>

#include <toolbox/logger/LoggerManager.h>
#include <toolbox/logger/Loggers.h>


#define _CONFIG_BASIC_LOGGERS \
      typedef std::shared_ptr<toolbox::Logger> LoggerPtr;\
      std::vector<LoggerPtr> _basic_loggers_loggers;\
      _basic_loggers_loggers.push_back(LoggerPtr(new toolbox::ConsoleLogger));\
      for (unsigned int i = 0; i < _basic_loggers_loggers.size(); ++i) {\
          toolbox::LoggerManager::instance().addLogger(_basic_loggers_loggers[i].get());\
      }\
      toolbox::LoggerManager::instance().configureLevel(toolbox::LogLevel::LL_0);\


#define MARK_VAR_UNUSED(varname) (void)varname



#if defined(_WIN32) || defined(CYGWIN)

#elif defined(linux) || defined(_linux) || defined(__linux) || defined(__linux__) || defined(__APPLE__)
#  ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#  endif
#  include <cstring>
#else
#  error "Unsupported platform, aborting compilation."
#endif
// Relative file name string, i.e. without path.
#ifndef __FILENAME__
#   define  __FILENAME__  (basename(__FILE__))
#endif


#if defined(DEBUG) || defined(USE_DEBUG) || defined(DEBUG_LOG_ENABLE)

    #include <assert.h>
    #include <string>
    #include <iostream>
    #include <sstream>
    #include <stdio.h>
    #include <cstdio>

#define PPFIELD(field) "{ " #field ": " << field << " }"


    #define LOG_INFO(x) {\
      std::stringstream ss;\
      ss << "[" << __FILE__ << ", " << __FUNCTION__ << "," << __LINE__ << "]: " << x;\
      toolbox::LoggerManager::instance().logMessage(ss.str().c_str(), toolbox::LogType::LT_INFO);\
      }

    #define LOG_OPTIMIZATION(x) {\
      std::stringstream ss;\
      ss << "[" << __FILE__ << ", " << __FUNCTION__ << "," << __LINE__ << "]: " << x;\
      toolbox::LoggerManager::instance().logMessage(ss.str().c_str(), toolbox::LogType::LT_OPTIMIZATION);\
    }


    #define LOG_ERROR(x) {\
      std::stringstream ss;\
      ss << "[" << __FILE__ << ", " << __FUNCTION__ << "," << __LINE__ << "]: " << x;\
      toolbox::LoggerManager::instance().logMessage(ss.str().c_str(), toolbox::LogType::LT_ERROR);\
      }

    #define LOG_WARNING(x) {\
      std::stringstream ss;\
      ss << "[" << __FILE__ << ", " << __FUNCTION__ << "," << __LINE__ << "]: " << x;\
      toolbox::LoggerManager::instance().logMessage(ss.str().c_str(), toolbox::LogType::LT_WARNING);\
      }

    #define LOG_TODO(x) {\
      std::stringstream ss;\
      ss << "[" << __FILE__ << ", " << __FUNCTION__ << "," << __LINE__ << "]: " << x;\
      toolbox::LoggerManager::instance().logMessage(ss.str().c_str(), toolbox::LogType::LT_TODO);\
      }

    #define LOG_STATUS(x) {\
      std::stringstream ss;\
      ss << "[" << __FILE__ << ", " << __FUNCTION__ << "," << __LINE__ << "]: " << x;\
      toolbox::LoggerManager::instance().logMessage(ss.str().c_str(), toolbox::LogType::LT_STATUS);\
      }





// common stuff
//
#define ASSERT_CMD(x, cmd) {const bool condition = (x); if(!condition){LOG_ERROR("Assert failed " #x "\n"); cmd; assert(false);}}
#define ASSERT(x)   {const bool condition = (x); if(!condition){LOG_ERROR("Assert failed " #x "\n"); assert(false);}}
#define ASSERT_PTR(x) ASSERT(x != nullptr)


#else
    #define ASSERT_CMD(x, cmd)
    #define ASSERT(x)
    #define ASSERT_PTR(x)
    #define LOG_OPTIMIZATION(format, ...)
    #define LOG_ERROR(format, ...)
    #define LOG_WARNING(format, ...)
    #define LOG_TODO(format, ...)
    #define LOG_INFO(format, ...)
    #define LOG_STATUS(format, ...)
    #define PPFIELD(x)

#endif

#endif
