#include "filter.h"
#include "object.h"
#include "objecttypetemplate.h"
#include "objectscope.h"
#include "interpreter.h"
#include "variable.h"


Filter::Filter(Interpreter* interpreter): _interpreter(interpreter)
{
}

bool Filter::setExpression(const std::string &expression)
{
    bool success = !expression.empty() && interpreter().loadFromString(expression);
    if(success)
    {
        _expression = expression;
        return true;
    }
    else
    {
        _expression = "";
        interpreter().clear();
        return false;
    }
}

const std::string &Filter::expression()
{
    return _expression;
}

bool Filter::filterChildren(Object& object)
{

    if(_expression != "")
    {
        return interpreter().evaluate(interpreter().program().elem(0), ConstObjectScope(object)).value().toBool();
    }
    else
        return true;
}

Interpreter &Filter::interpreter()
{
    return *_interpreter;
}
