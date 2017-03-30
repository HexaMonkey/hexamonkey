#ifndef METHODSCOPE_H
#define METHODSCOPE_H

#include "core/variable/variable.h"

class MethodScope : public VariableImplementation
{
public:
    MethodScope(VariableArgs &args, VariableKeywordArgs &kwargs, VariableCollector& collector);

    virtual Variable doGetField(const Variant &key, bool, bool createIfNeeded) override;

private:
   VariableArgs& _args;
   VariableKeywordArgs& _kwargs;
};

#endif // METHODSCOPE_H
