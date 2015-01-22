#include "logger.h"
#include "logmanager.h"

Logger::Logger()
{
    LogManager::getInstance()->attach(this);
}

Logger::~Logger()
{
    LogManager::getInstance()->detach(this);
}


