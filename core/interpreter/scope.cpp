#include "scope.h"

Variant* Scope::declare(const Variant &key) const
{
    return doDeclare(key);
}

Scope *Scope::getScope(const Variant &key) const
{
    return doGetScope(key);
}

Variant *Scope::doDeclare(const Variant &/*key*/) const
{
    return nullptr;
}

Scope *Scope::doGetScope(const Variant &/*key*/) const
{
    return nullptr;
}

Variant *MutableScope::get(const Variant &key) const
{
    return doGet(key);
}

const Variant *MutableScope::cget(const Variant &key) const
{
    return doGet(key);
}


Variant *ConstScope::get(const Variant &/*key*/) const
{
    return nullptr;
}

const Variant *ConstScope::cget(const Variant &key) const
{
    return doCget(key);
}

Variant *EmptyScope::doGet(const Variant &/*key*/) const
{
    return nullptr;
}
