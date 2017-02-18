#include "methodscope.h"

#include "core/log/logmanager.h"

MethodScope::MethodScope(VariableArgs &args, VariableKeywordArgs &kwargs, VariableCollector &collector)
    : VariableImplementation(collector),
      _args(args),
      _kwargs(kwargs)

{
}

Variable MethodScope::doGetField(const Variant &key, bool, bool createIfNeeded)
{
    if (key.hasNumericalType()) {
        int64_t number = key.toInteger();
        if (number >= 0 || number < _args.size()) {
            return _args[number];
        } else {
            if (createIfNeeded) {
                Log::error("Trying to assign a numbered parameter value outside of the bounds");
            }
            return Variable();
        }
    } else if (key.type() == Variant::stringType) {
        const std::string& name = key.toString();
        auto it = _kwargs.find(name);
        if (it != _kwargs.end()) {
            return it->second;
        } else {
            if (createIfNeeded) {
                Log::error("Trying to assign a numbered attribute value outside of the bounds");
            }
            return Variable();
        }
    } else {
        if (createIfNeeded) {
            Log::error("Trying to assign an attribute with an invalid key type");
        }
        return Variable();
    }
}

bool MethodScope::isByRefOnly()
{
    return true;
}
