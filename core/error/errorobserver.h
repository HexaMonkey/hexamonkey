#ifndef ERROROBSERVER_H
#define ERROROBSERVER_H

#include <string>

class ErrorObserver
{
public:
    virtual void update(std::string errorRaised) = 0;
};

#endif // ERROROBSERVER_H
