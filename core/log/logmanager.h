#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <vector>
#include <string>
#include <sstream>
#include "core/log/logger.h"
#include "core/util/strutil.h"

class LogManager
{
private:
    static bool instanceFlag;
    static LogManager *single;
    LogManager();
    std::vector<Logger*> errorObserverList;
public:
    static LogManager* getInstance();
    ~LogManager();
    void attach(Logger* errorObserver);
    void detach(Logger* errorObserver);

    void log(const std::string& message, LogLevel level);
};

namespace Log {
    template <typename T, typename ...P>
    void info(const T& t, const P& ...p)
    {
        LogManager::getInstance()->log(concat(t, p...), LogLevel::Info);
    }

    template <typename T, typename ...P>
    void warning(const T& t, const P& ...p)
    {
        LogManager::getInstance()->log(concat(t, p...), LogLevel::Warning);
    }

    template <typename T, typename ...P>
    void error(const T& t, const P& ...p)
    {
        LogManager::getInstance()->log(concat(t, p...), LogLevel::Error);
    }
}

#endif // ERRORMANAGER_H
