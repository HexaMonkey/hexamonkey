#ifndef ERROROBSERVER_H
#define ERROROBSERVER_H

#include <string>

enum class LogLevel {Info = 2, Warning = 1, Error = 0};

class Logger
{
public:
    Logger();
    ~Logger();
    virtual void update(const std::string& errorRaised, LogLevel level) = 0;
};

#endif // ERROROBSERVER_H
