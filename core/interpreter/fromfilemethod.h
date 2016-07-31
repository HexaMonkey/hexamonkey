#ifndef FROMFILEMETHOD_H
#define FROMFILEMETHOD_H

#include "core/modulemethod.h"

#include "core/interpreter/program.h"

class FromFileMethod : public ModuleMethod
{
public:
    template <typename TSignature>
    FromFileMethod(Program definition, TSignature&& signature, const Module& module)
        : ModuleMethod(signature),
          _definition(definition),
          _module(module)
    {
    }

    virtual Variable call(VariableArgs& args, VariableKeywordArgs& kwargs, VariableCollector& collector) const override;

private:
    Program _definition;
    const Module& _module;
};

#endif // FROMFILEMETHOD_H
