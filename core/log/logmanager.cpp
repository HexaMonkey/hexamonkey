#include "logmanager.h"
#include <iostream>

bool LogManager::instanceFlag = false;
LogManager* LogManager::single = NULL;

LogManager::LogManager()
{
    //empty
}

LogManager::~LogManager()
{
    instanceFlag = false;
}

LogManager* LogManager::getInstance()
{
    if(! instanceFlag)
    {
        single = new LogManager();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}

void LogManager::attach(Logger* errorObserver)
{
    errorObserverList.push_back(errorObserver);
}

void LogManager::detach(Logger* errorObserver)
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

void LogManager::log(const std::string &message, LogLevel level)
{
    for(unsigned int i=0; i < errorObserverList.size();i++)
    {
        errorObserverList[i]->update(message, level);
    }
}

