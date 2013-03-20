#ifndef FILTER_H
#define FILTER_H

#include <memory>
#include <string>
#include "interpreter.h"

class Object;

class Filter
{
public:
    Filter(Interpreter* interpreter);

    bool setExpression(const std::string& expression);
    const std::string& expression();

    bool filterChildren(Object &object);

private:
    Interpreter& interpreter();

    std::unique_ptr<Interpreter> _interpreter;
    std::string _expression;
};

#endif // FILTER_H
