#include <toolbox/logger/LoggerManager.h>


namespace toolbox {


LoggerManager&
LoggerManager::instance(void)
{
    static LoggerManager instance;
    return instance;
}

} // namespace toolbox
