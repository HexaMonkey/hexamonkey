#include "interpreterconstructor.h"

#include "interpreter.h"
#include "objecttypetemplate.h"

InterpreterConstructor::InterpreterConstructor(const HmcModule &hmcModule)
    : _hmcModule(hmcModule)
{
}

Interpreter *InterpreterConstructor::newInstance() const
{
    return new Interpreter(_hmcModule);
}
