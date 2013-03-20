#include "variable.h"
#include "variant.h"

Variable::~Variable()
{
    if(_own)
    {
        delete _var;
    }
}

Variant &Variable::value()
{
    if(_constant)
    {
        Variant* newValue = new Variant(*_constVar);
        _own = true;
        _constant = false;
        _var = newValue;
        _constVar = newValue;
    }
    return *_var;
}

const Variant &Variable::constValue() const
{
    return *_constVar;
}

Variable *Variable::copy(const Variant &value)
{
    Variant* newValue = new Variant(value);
    return new Variable(true, false, newValue, newValue);
}

Variable *Variable::move(Variant *value)
{
    return new Variable(true, false, value, value);
}

Variable *Variable::constReference(const Variant &value)
{
    return new Variable(false, true, NULL, &value);
}

Variable *Variable::reference(Variant &value)
{
    return new Variable(false, false, &value, &value);
}

Variable::Variable(bool own, bool constant , Variant* var, const Variant* constVar)
    : _own(own),
      _constant(constant),
      _var(var),
      _constVar(constVar)
{
}
