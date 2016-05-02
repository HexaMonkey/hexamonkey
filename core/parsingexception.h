#ifndef PARSINGEXCEPTION_H
#define PARSINGEXCEPTION_H


#include <exception>
#include <string>

class ParsingException : std::exception
{
public:
    enum Type {
        OutOfFile,
        OutOfParent,
        InvalidChild,
        BadParameter
    };

    ParsingException(Type type, const std::string& message);

    const char* what() const noexcept override;

    Type type() const;

private:
    Type _type;
    std::string _message;
};

#endif // PARSINGEXCEPTION_H
