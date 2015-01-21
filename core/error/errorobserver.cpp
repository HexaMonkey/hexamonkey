#include "errorobserver.h"
#include "errormanager.h"

ErrorObserver::ErrorObserver()
{
    ErrorManager::getInstance()->attach(this);
}

ErrorObserver::~ErrorObserver()
{
    ErrorManager::getInstance()->detach(this);
}


