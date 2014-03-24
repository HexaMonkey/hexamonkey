#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <vector>
#include <string>
#include <sstream>
#include "core/error/errorobserver.h"

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
    std::ostringstream errorMessage;
};

void operator<<(ErrorManager * errorManager, std::ostringstream stream);

#endif // ERRORMANAGER_H
