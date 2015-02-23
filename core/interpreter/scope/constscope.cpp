#include "constscope.h"

ConstScope::ConstScope(Scope::Ptr scopePtr)
    : _scopePtr(scopePtr),
      _scope(*scopePtr.get())
{
}

Variable ConstScope::doGet(const Variant &key, bool /*modifiable*/)
{
    return _scope.get(key, false);
}

Scope::Ptr ConstScope::doGetScope(const Variant &key)
{
    return Scope::Ptr(new ConstScope(_scope.getScope(key)));
}

Variable ConstScope::doDeclare(const Variant &/*key*/)
{
    return Variable();
}

Variable ConstScope::getValue(bool /*modifiable*/)
{
    return _scope.getValue(false);
}
