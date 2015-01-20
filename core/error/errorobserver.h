#ifndef ERROROBSERVER_H
#define ERROROBSERVER_H

#include <string>

enum class LogLevel {Info = 2, Warning = 1, Error = 0};

class ErrorObserver
{
public:
    virtual void update(std::string errorRaised, LogLevel level) = 0;
};

#endif // ERROROBSERVER_H
