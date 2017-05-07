/*
 * File Loggers.h,
 * Created on 02/12/2014.
 * Author: Agustin
 *
 * Happen
*/

#ifndef LOGGERS_H
#define LOGGERS_H

#include <fstream>
#include <string>
#include <iostream>

#include "LoggerManager.h"


namespace core {


////////////////////////////////////////////////////////////////////////////////
///
/// \brief The FileLogger class will log into a file
///
class FileLogger : public Logger {
public:
    FileLogger(const std::string& filename, bool append = true)
    {
        m_file.open(filename.c_str(), append ? std::ios_base::app : std::ios_base::out);
    }
    ~FileLogger()
    {
        m_file.flush();
    }

    void
    logMessage(const char* msg, LogType lt)
    {
        // this should match to the core::LogType
        static const char* LOGTYPE_STRINGS[] = {
            "[ERROR] ",
            "[WARNING] ",
            "[TODO] ",
            "[OPTIMIZATION] ",
            "[INFO] ",
            "[STATUS] "
        };
        m_file << LOGTYPE_STRINGS[lt] << msg << std::endl;
        m_file.flush(); // just in case
    }

private:
    std::ofstream m_file;
};


#define CONSOLE_BLACK       "\33[22;30m"
#define CONSOLE_GRAY 		"\33[01;30m"
#define CONSOLE_RED 		"\33[22;31m"
#define CONSOLE_LRED	 	"\33[01;21m"
#define CONSOLE_GREEN       "\33[22;32m"
#define CONSOLE_LGREEN      "\33[01;32m"
#define CONSOLE_BLUE        "\33[22;34m"
#define CONSOLE_BROWN       "\33[22;33m"
#define CONSOLE_YELLOW      "\33[00;33m"
#define CONSOLE_CYAN 		"\33[22;36m"
#define CONSOLE_LCYAN	 	"\33[22;36m"
#define CONSOLE_MAGENTA 	"\33[22;35m"
#define CONSOLE_LMAGENTA 	"\33[01;35m"
#define CONSOLE_NC          "\33[0m"
#define CONSOLE_BOLD        "\33[1m"
#define CONSOLE_ULINE       "\33[4m" //underline
#define CONSOLE_BLINK       "\33[5m"
#define CONSOLE_INVERT      "\33[7m"


////////////////////////////////////////////////////////////////////////////////
///
/// \brief The ConsoleLogger class will log into the console
///
class ConsoleLogger : public Logger {
public:
    ConsoleLogger()
    {
    }
    ~ConsoleLogger()
    {
    }

    void
    logMessage(const char* msg, LogType lt)
    {
        // this should match to the core::LogType
        static const char* LOGTYPE_STRINGS[] = {
            // it is not supported by the qt console now, in linux should
            // show this nicely
            CONSOLE_INVERT CONSOLE_RED  "[ERROR] ",
            CONSOLE_RED CONSOLE_ULINE   "[WARNING] ",
            CONSOLE_BOLD CONSOLE_YELLOW "[TODO] ",
            CONSOLE_MAGENTA             "[OPTIMIZATION] ",
            CONSOLE_NC                  "[INFO] ",
            CONSOLE_GREEN CONSOLE_BOLD  "[STATUS] "
//            "[ERROR] ",
//            "[WARNING] ",
//            "[TODO] ",
//            "[OPTIMIZATION] ",
//            "[INFO] ",
//            "[STATUS] "
        };
        std::cout << LOGTYPE_STRINGS[lt] << msg << std::endl;
    }

private:
    std::ofstream m_file;
};

}

#endif // LOGGERS_H
