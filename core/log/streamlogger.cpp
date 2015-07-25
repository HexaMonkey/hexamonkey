#include "streamlogger.h"

StreamLogger::StreamLogger(std::ostream &stream) : _stream(stream)
{

}

void StreamLogger::update(const std::string &errorRaised, LogLevel level)
{
    switch (level) {
        case LogLevel::Info:
            _stream << "[INFO]    ";
            break;

        case LogLevel::Warning:
            _stream << "[WARNING] ";
            break;

        case LogLevel::Error:
            _stream << "[ERROR]   ";
            break;
    }
    _stream << errorRaised << std::endl;
}
