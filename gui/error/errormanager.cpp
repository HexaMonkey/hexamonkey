#include "errormanager.h"
#include <iostream>

bool ErrorManager::instanceFlag = false;
ErrorManager* ErrorManager::single = NULL;

ErrorManager::ErrorManager()
{
    //empty
}

ErrorManager::~ErrorManager()
{
    instanceFlag = false;
}

ErrorManager* ErrorManager::getInstance()
{
    if(! instanceFlag)
    {
        single = new ErrorManager();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}


void ErrorManager::attach(ErrorObserver* errorObserver)
{
    errorObserverList.push_back(errorObserver);
}

void ErrorManager::detach(ErrorObserver* errorObserver)
{
    for(unsigned int i=0; i < errorObserverList.size();i++)
    {
        if (errorObserver == errorObserverList[i])
        {
            errorObserverList.erase(errorObserverList.begin()+i);
            break;
        }
    }
}

void ErrorManager::notify()
{
    for(unsigned int i=0; i < errorObserverList.size();i++)
    {
        errorObserverList[i]->update(errorMessage.str());
    }
    errorMessage.flush();
}

/*void operator<<(ErrorManager * errorManager, std::ostringstream stream)
{
    errorManager->notify(stream.str());
}*/
