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
    const size_t nArgs = args.size();
    const size_t nSignature = _signature.size();
    size_t i = 0;
    for (; i < nArgs && i < nSignature; ++i)
    {
        const MethodArgument& argSignature = _signature[i];
        if (argSignature.constant()) {
            args[i].setConstant();
        }
    }

    for (; i < nSignature; ++i)
    {
        const MethodArgument& argSignature = _signature[i];
        auto it = kwargs.find(argSignature.name());
        if (it != kwargs.cend()) {
            args.push_back(it->second);
        } else {
            args.push_back(collector.copy(argSignature.defaultValue()));
        }

        if (argSignature.constant()) {
            args[i].setConstant();
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
        if (it != kwargs.cend()) {
            if (argSignature.constant()) {
                it->second.setConstant();
            }
        } else {
            kwargs[argSignature.name()] = args[i];
        }
    }
}
