#ifndef STREAMLOGGER_H
#define STREAMLOGGER_H

#include "core/log/logger.h"

#include <ostream>

class StreamLogger : public Logger
{
public:
    StreamLogger(std::ostream& stream);
    virtual void update(const std::string& errorRaised, LogLevel level) override;

private:
    std::ostream& _stream;
};

#endif // STREAMLOGGER_H
