#include "modulemethod.h"

ModuleMethod::ModuleMethod()
{

}

Variable ModuleMethod::call(VariableArgs &, VariableKeywordArgs &, VariableCollector &) const
{
    return Variable();
}

void ModuleMethod::fillNumberedArgs(VariableArgs &args, const VariableKeywordArgs &kwargs, VariableCollector &collector) const
{
    const size_t nSignature = _signature.size();
    size_t i = 0;

    for (; i < nSignature; ++i)
    {
        const MethodArgument& argSignature = _signature[i];
        auto it = kwargs.find(argSignature.name());
        if (it != kwargs.cend()) {
            args.push_back(it->second);
        } else {
            args.push_back(collector.copy(argSignature.defaultValue()));
        }
    }
}

void ModuleMethod::fillArgs(VariableArgs &args, VariableKeywordArgs &kwargs, VariableCollector &collector) const
{

    fillNumberedArgs(args, kwargs, collector);

    for (size_t i = 0, nSignature = _signature.size(); i < nSignature; ++i)
    {
        const MethodArgument& argSignature = _signature[i];
        auto it = kwargs.find(argSignature.name());
        if (it == kwargs.cend()) {
            kwargs[argSignature.name()] = args[i];
        }
    }
}
