#include "holder.h"
#include "interpreter.h"
#include "variable.h"


Holder::Holder(Interpreter& interpreter)
    : _interpreter(interpreter)
{
}

Holder::~Holder()
{
    while(!_held.empty())
    {
        auto it = _held.begin();
        _interpreter.release(*it->second);
        _held.erase(it);
    }
}

Variant &Holder::getNew()
{
    Variable& var = _interpreter.null();
    add(var);
    return var.value();
}

Variable &Holder::add(Variable &var)
{
    _held.insert(std::make_pair(&var.cvalue(), &var));
    return var;
}

Variant &Holder::evaluate(const Program &rightValue, const Scope &scope, const Module &module)
{
    Variable& var = _interpreter.evaluate(rightValue, scope, module);
    add(var);
    return var.value();
}

const Variant &Holder::cevaluate(const Program &rightValue, const Scope &scope, const Module &module)
{
    Variable& var = _interpreter.evaluate(rightValue, scope, module);
    add(var);
    return var.cvalue();
}


Variable &Holder::extract(Variable &var)
{
    auto range = _held.equal_range(&var.cvalue());

    for(auto it = range.first; it != range.second; ++it)
    {
        Variable& extracted = *it->second;\
        if(extracted.isOwner())
        {
            _held.erase(it);
            return extracted;
        }
    }

    if(range.first != range.second)
    {
        Variable& extracted = *range.first->second;\
        _held.erase(range.first);
        return extracted;
    }

    return var;
}
