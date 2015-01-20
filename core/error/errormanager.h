#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <vector>
#include <string>
#include <sstream>
#include "core/error/errorobserver.h"
#include "core/util/strutil.h"

class ErrorManager
{
private:
    static bool instanceFlag;
    static ErrorManager *single;
    ErrorManager();
    std::vector<ErrorObserver*> errorObserverList;
public:
    static ErrorManager* getInstance();
    ~ErrorManager();
    void attach(ErrorObserver* errorObserver);
    void detach(ErrorObserver* errorObserver);
    void notify();
    void notify(std::string errorString);
    std::ostringstream errorMessage;

    void log(const std::string& message, LogLevel level);
};

namespace Log {
    template <typename T, typename ...P>
    void info(T t, P ...p)
    {
        ErrorManager::getInstance()->log(concat(t, p...), LogLevel::Info);
    }

    template <typename T, typename ...P>
    void warning(T t, P ...p)
    {
        ErrorManager::getInstance()->log(concat(t, p...), LogLevel::Warning);
    }

    template <typename T, typename ...P>
    void error(T t, P ...p)
    {
        ErrorManager::getInstance()->log(concat(t, p...), LogLevel::Error);
    }
}

void operator<<(ErrorManager * errorManager, std::ostringstream stream);

#endif // ERRORMANAGER_H
