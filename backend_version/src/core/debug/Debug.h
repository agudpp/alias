/*
*  Useful functions used in debug
*/

#ifndef DEBUGUTIL_H_
#define DEBUGUTIL_H_

#include <functional>
#include <vector>

#include <core/logger/LoggerManager.h>
#include <core/logger/Loggers.h>


#define _CONFIG_BASIC_LOGGERS \
      typedef std::shared_ptr<core::Logger> LoggerPtr;\
      std::vector<LoggerPtr> _basic_loggers_loggers;\
      _basic_loggers_loggers.push_back(LoggerPtr(new core::ConsoleLogger));\
      for (unsigned int i = 0; i < _basic_loggers_loggers.size(); ++i) {\
          core::LoggerManager::instance().addLogger(_basic_loggers_loggers[i].get());\
      }\
      core::LoggerManager::instance().configureLevel(core::LogLevel::LL_0);\


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


#if defined(DEBUG) || 1 || defined(DEBUG_LOG_ENABLE)

    #include <assert.h>
    #include <iostream>
    #include <stdio.h>
    #include <cstdio>


// shared buffer for the debugg information. THIS IS NOT THREAD SAFE!!!!!
// be carefoul when logging from different threads...
    #define _SHRD_DEBUG_BUFF_SIZE 8192
    static char _shrd_debug_buff[_SHRD_DEBUG_BUFF_SIZE+1];


    #define debugINFO(format, ...) {\
                    const int _len = snprintf(_shrd_debug_buff, _SHRD_DEBUG_BUFF_SIZE, "[%s, %s, %d]: " format, \
                     __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__);\
                     if (_len < _SHRD_DEBUG_BUFF_SIZE && _len >= 0) {\
                        core::LoggerManager::instance().logMessage(_shrd_debug_buff, core::LogType::LT_INFO);\
                     }\
                    }

    #define debugOPTIMIZATION(format, ...) {\
                    const int _len = snprintf(_shrd_debug_buff, _SHRD_DEBUG_BUFF_SIZE, "[%s, %s, %d]: " format, \
                     __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__);\
                     if (_len < _SHRD_DEBUG_BUFF_SIZE && _len >= 0) {\
                     core::LoggerManager::instance().logMessage(_shrd_debug_buff, core::LogType::LT_OPTIMIZATION);\
                     }\
                    }

    #define debugERROR(format, ...) {\
                    const int _len = snprintf(_shrd_debug_buff, _SHRD_DEBUG_BUFF_SIZE, "[%s, %s, %d]: " format, \
                     __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__);\
                     if (_len < _SHRD_DEBUG_BUFF_SIZE && _len >= 0) {\
                     core::LoggerManager::instance().logMessage(_shrd_debug_buff, core::LogType::LT_ERROR);\
                     }\
                    }

    #define debugWARNING(format, ...) {\
                    const int _len = snprintf(_shrd_debug_buff, _SHRD_DEBUG_BUFF_SIZE, "[%s, %s, %d]: " format, \
                     __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__);\
                     if (_len < _SHRD_DEBUG_BUFF_SIZE && _len >= 0) {\
                     core::LoggerManager::instance().logMessage(_shrd_debug_buff, core::LogType::LT_WARNING);\
                     }\
                    }

    #define debugTODO(format, ...) {\
                    const int _len = snprintf(_shrd_debug_buff, _SHRD_DEBUG_BUFF_SIZE, "[%s, %s, %d]: " format, \
                     __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__);\
                     if (_len < _SHRD_DEBUG_BUFF_SIZE && _len >= 0) {\
                     core::LoggerManager::instance().logMessage(_shrd_debug_buff, core::LogType::LT_TODO);\
                     }\
                    }

    #define debugSTATUS(format, ...) {\
                     const int _len = snprintf(_shrd_debug_buff, _SHRD_DEBUG_BUFF_SIZE, "[%s, %s, %d]: " format, \
                     __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__);\
                     if (_len < _SHRD_DEBUG_BUFF_SIZE && _len >= 0) {\
                     core::LoggerManager::instance().logMessage(_shrd_debug_buff, core::LogType::LT_STATUS);\
                     }\
                    }





// common stuff
//
#define ASSERT(x)   {const bool condition = (x); if(!condition){debugERROR("Assert failed " #x "\n"); assert(false);}}
#define ASSERT_PTR(x) ASSERT(x != nullptr)
#define OGRELOG(x)  std::cerr << "OGRELOG: " << (x) << std::endl;


#else
    #define ASSERT(x)
    #define ASSERT_PTR(x)
    #define OGRELOG(x)
    #define debugOPTIMIZATION(format, ...)
    #define debugERROR(format, ...)
    #define debugWARNING(format, ...)
    #define debugTODO(format, ...)
    #define debugINFO(format, ...)
    #define debugSTATUS(format, ...)
#endif

#endif
