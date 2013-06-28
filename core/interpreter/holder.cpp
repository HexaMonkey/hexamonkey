#include "holder.h"
#include "interpreter.h"
#include "variable.h"


Holder::Holder(Program program)
    : _program(program)
{
}

Holder::~Holder()
{
    while(!_held.empty())
    {
        auto it = _held.begin();
        _program.releaseVariable(*it->second);
        _held.erase(it);
    }
}

Variant &Holder::getNew()
{
    Variable& var = _program.null();
    add(var);
    return var.value();
}

Variant &Holder::copy(const Variant &value)
{
    Variable& var = _program.copy(value);
    add(var);
    return var.value();
}

Variable &Holder::add(Variable &variable)
{
    _held.insert(std::make_pair(&variable.cvalue(), &variable));
    return variable;
}

Variant &Holder::evaluate(const Scope &scope, const Module &module)
{
    Variable& var = _program.evaluate(scope, module);
    add(var);
    return var.value();
}

const Variant &Holder::cevaluate(const Scope &scope, const Module &module)
{
    Variable& var = _program.evaluate(scope, module);
    add(var);
    return var.cvalue();
}


Variable &Holder::extract(Variable &variable)
{
    auto range = _held.equal_range(&variable.cvalue());

    //Looks through the variable held with the same variant
    //and returns an owner
    for(auto it = range.first; it != range.second; ++it)
    {
        Variable& extracted = *it->second;\
        if(extracted.isOwner())
        {
            _held.erase(it);
            return extracted;
        }
    }

    //If there is no owner, return any reference
    if(range.first != range.second)
    {
        Variable& extracted = *range.first->second;\
        _held.erase(range.first);
        return extracted;
    }

    return variable;
}
