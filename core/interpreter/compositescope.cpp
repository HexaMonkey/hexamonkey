#include "compositescope.h"

void CompositeScope::addScope(Scope &scope)
{
    _scopes.push_back(&scope);
}

Variant *CompositeScope::get(const Variant &key) const
{
    for(Scope* scope : _scopes)
    {
        Variant* variant = scope->get(key);
        if(variant != nullptr)
            return variant;
    }
    return nullptr;
}

const Variant *CompositeScope::cget(const Variant &key) const
{
    for(Scope* scope : _scopes)
    {
        const Variant* variant = scope->cget(key);
        if(variant != nullptr)
            return variant;
    }
    return nullptr;
}

Scope *CompositeScope::doGetScope(const Variant &key) const
{
    for(Scope* scope : _scopes)
    {
        Scope* s = scope->getScope(key);
        if(s != nullptr)
            return s;
    }
    return nullptr;
}

Variant* CompositeScope::doDeclare(const Variant &key) const
{
    for(Scope* scope : _scopes)
    {
        Variant* variant = scope->declare(key);
        if(variant != nullptr)
            return variant;
    }
    return nullptr;
}
