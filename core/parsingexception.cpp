#include "parsingexception.h"

ParsingException::ParsingException(Type type, const std::string &message) : _type(type), _message(message)
{
}

const char *ParsingException::what() const noexcept
{
    return _message.c_str();
}

ParsingException::Type ParsingException::type() const
{
    return _type;
}
